#!/usr/bin/env python3
"""Validate and time each selected solution ONCE, with no warm-up.

Compilation is a separate Make target. Timings include process/interpreter
startup, computation, output capture and teardown. Test runs are kept apart
from profiling runs; each run has its own directory and provenance metadata.
Profiling additionally calibrates a *minimal* language startup baseline using
no-op programs, not extra runs of the Euler solutions. It is only an estimate.
"""
from __future__ import annotations

import argparse
import csv
from datetime import datetime, timezone
import hashlib
import json
import math
import os
from pathlib import Path
import platform
import re
import shlex
import shutil
import subprocess
import sys
import time
import uuid

# Running as `python scripts/bench.py` and importlib-loading it in the tests
# establish different import roots. Both use the same shared implementation.
try:
    from scripts.startup_timing import calibrate, isolated_factor_environment, split_wall_time
except ModuleNotFoundError:
    from startup_timing import calibrate, isolated_factor_environment, split_wall_time

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "tests" / "answers.json"
LANGUAGES = ("c", "rust", "forth", "factor")
def selected_problems(language: str, manifest: list[dict]) -> list[dict]:
    """Honor explicit language coverage from the answer manifest."""
    if language not in LANGUAGES:
        raise ValueError(f"Unsupported language: {language}")
    return [p for p in manifest if language in p.get("languages", ())]

def factor_vm() -> str:
    # GNU coreutils also provides /usr/bin/factor; do not invoke that.
    return os.environ.get("FACTOR_VM") or str(ROOT / ".toolchains/factor/factor")
FIELDS = ("run_id", "language", "problem", "stem", "runtime_ms",
          "startup_estimate_ms", "execution_residual_ms", "split_status",
          "status", "expected", "actual", "exit_code", "message")


def command(language: str, number: int, stem: str) -> list[str]:
    suffix = ".exe" if os.name == "nt" else ""
    if language == "c":
        return [str(ROOT / "build" / "c" / (stem + suffix))]
    if language == "rust":
        return [str(ROOT / "solutions" / "rust" / "target" / "release" /
                    (f"p{number}" + suffix))]
    if language == "forth":
        return ["gforth", str(ROOT / "solutions" / "forth" / f"p{number}.fs")]
    if language == "factor":
        return [factor_vm(), f"-roots={ROOT / 'solutions' / 'factor'}",
                str(ROOT / "solutions" / "factor" / f"p{number}.factor")]
    raise ValueError(f"Unsupported language: {language}")


def probe(argv: list[str]) -> str:
    try:
        proc = subprocess.run(argv, cwd=ROOT, stdout=subprocess.PIPE,
                              stderr=subprocess.STDOUT, text=True, timeout=5)
        return proc.stdout.strip() if proc.returncode == 0 else "unavailable"
    except (OSError, subprocess.TimeoutExpired):
        return "unavailable"


def cpu_name() -> str:
    path = Path("/proc/cpuinfo")
    if path.is_file():
        for line in path.read_text(errors="replace").splitlines():
            if line.startswith("model name") and ":" in line:
                return line.split(":", 1)[1].strip()
    return platform.processor() or "unavailable"


def environment() -> dict:
    cc = shlex.split(os.environ.get("CC", "gcc"))
    status = probe(["git", "status", "--porcelain", "--untracked-files=no"])
    return {
        "os": platform.platform(), "machine": platform.machine(),
        "cpu": cpu_name(), "logical_cpus": os.cpu_count(),
        "python": sys.version, "revision": probe(["git", "rev-parse", "HEAD"]),
        "tracked_changes": status if status != "unavailable" else None,
        "cc": probe(cc + ["--version"]).splitlines()[0],
        "cflags": os.environ.get("CFLAGS", "-O2 -std=c11 -Wall -Wextra"),
        "rustc": probe(["rustc", "--version"]),
        "cargo": probe(["cargo", "--version"]),
        "rust_profile": "release (see solutions/rust/Cargo.toml)",
        "gforth": probe(["gforth", "--version"]),
        "factor_vm": factor_vm(),
        "github_run_id": os.environ.get("GITHUB_RUN_ID"),
        "manifest_sha256": hashlib.sha256(MANIFEST.read_bytes()).hexdigest(),
    }


def run_one(language: str, problem: dict, timeout: float, run_id: str,
            startup_calibration: dict | None = None) -> dict:
    number, stem, expected = (problem[k] for k in ("number", "stem", "answer"))
    cmd = command(language, number, stem)
    row = dict(run_id=run_id, language=language, problem=number, stem=stem,
               runtime_ms="", startup_estimate_ms="", execution_residual_ms="",
               split_status="unavailable", status="missing", expected=str(expected),
               actual="", exit_code="", message="")
    if not shutil.which(cmd[0]) and not Path(cmd[0]).is_file():
        row["message"] = "executable/interpreter not found; no execution occurred"
        return row
    start = time.perf_counter_ns()
    try:
        # This is the sole solution invocation. Validation uses this same output.
        kwargs = ({"env": isolated_factor_environment(root=ROOT)}
                  if language == "factor" else {})
        proc = subprocess.run(cmd, cwd=ROOT, encoding="utf-8", errors="replace",
                              capture_output=True, timeout=timeout, check=False, **kwargs)
        elapsed = time.perf_counter_ns() - start
        row["runtime_ms"] = f"{elapsed / 1_000_000:.6f}"
        row["exit_code"] = proc.returncode
        stdout = proc.stdout.strip()
        row["actual"] = stdout[:256]
        row["message"] = proc.stderr.strip()[:500]
        if proc.returncode:
            row["status"] = "error"
        elif re.fullmatch(r"-?[0-9]+", stdout) is None:
            row["status"] = "invalid-output"
        elif stdout != str(expected):
            row["status"] = "wrong-answer"
        else:
            row["status"] = "pass"
        if row["status"] == "pass":
            (row["startup_estimate_ms"], row["execution_residual_ms"],
             row["split_status"]) = split_wall_time(row["runtime_ms"], startup_calibration)
    except subprocess.TimeoutExpired:
        row["runtime_ms"] = f"{(time.perf_counter_ns() - start) / 1_000_000:.6f}"
        row["status"] = "timeout"
        row["message"] = f"exceeded {timeout:g} [s]; not a completed solution"
    except OSError as exc:
        row["runtime_ms"] = f"{(time.perf_counter_ns() - start) / 1_000_000:.6f}"
        row["status"] = "error"
        row["message"] = str(exc)[:500]
    return row


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--languages", nargs="+", choices=LANGUAGES,
                        default=list(LANGUAGES))
    parser.add_argument("--mode", choices=("test", "profile"), default="test")
    parser.add_argument("--timeout", type=float, default=30.0)
    parser.add_argument("--startup-samples", type=int, default=5,
                        help="Number of independent no-op runs per language (profile only)")
    args = parser.parse_args(argv)
    if len(set(args.languages)) != len(args.languages):
        parser.error("Duplicate languages would violate the single-run policy")
    if not math.isfinite(args.timeout) or args.timeout <= 0:
        parser.error("--timeout must be a finite positive number")
    if not 1 <= args.startup_samples <= 99:
        parser.error("--startup-samples must be between 1 and 99")
    problems = json.loads(MANIFEST.read_text(encoding="utf-8"))["problems"]
    numbers = [p["number"] for p in problems]
    if len(numbers) != len(set(numbers)):
        parser.error("Duplicate problem numbers in the manifest")
    problems.sort(key=lambda p: p["number"])
    stamp = datetime.now(timezone.utc)
    run_id = stamp.strftime("%Y%m%dT%H%M%S.%fZ") + "-" + uuid.uuid4().hex[:8]
    base = ROOT / "build" / ("profile" if args.mode == "profile" else "test-results")
    outdir = base / run_id
    outdir.mkdir(parents=True, exist_ok=False)
    info = {
        "schema": 3, "run_id": run_id, "mode": args.mode,
        "timestamp_utc": stamp.isoformat(), "clock": "time.perf_counter_ns",
        "runs_per_solution": 1, "warmup_runs": 0,
        "compilation_included": False, "startup_included": True,
        "unit": "ms", "timeout_s": args.timeout, "languages": args.languages,
        "problems": sorted(numbers),
        "coverage": {lang: sorted(p["number"] for p in selected_problems(lang, problems))
                     for lang in args.languages},
        "environment": environment(),
        "startup_calibrations": {},
        "language_summaries": {},
        "interpretation": "One observation per program; no statistical uncertainty estimate. "
                          "Startup is a separately sampled no-op process baseline, NOT a direct "
                          "measurement of each solution's initialization. Execution residual "
                          "includes solver initialization, imports, computation, I/O and teardown. "
                          "If the baseline exceeds wall time the startup part is capped, not extrapolated. "
                          "Implementations differ in algorithms, libraries and input loading; "
                          "this is not an intrinsic language-speed comparison.",
    }
    failures = 0
    suite_start = time.perf_counter_ns()
    for language in args.languages:
        calibration = (calibrate(language, samples=args.startup_samples,
                                 timeout=args.timeout, factor_vm=factor_vm())
                       if args.mode == "profile" else None)
        info["startup_calibrations"][language] = calibration or {
            "status": "not-run", "message": "Tests do not perform startup calibration"
        }
        language_start = time.perf_counter_ns()
        subset = selected_problems(language, problems)
        rows = [run_one(language, item, args.timeout, run_id, calibration) for item in subset]
        language_wall = (time.perf_counter_ns() - language_start) / 1_000_000
        passed = sum(row["status"] == "pass" for row in rows)
        failures += len(rows) - passed
        child_ms = sum(float(r["runtime_ms"]) for r in rows if r["runtime_ms"])
        summary = {
            "passed": passed, "expected_count": len(rows),
            "complete": passed == len(rows),
            "full_coverage": len(rows) == len(problems),
            "manifest_count": len(problems),
            "suite_wall_ms": language_wall,
            "attempted_child_runtime_ms": child_ms,
            "passing_child_runtime_ms": sum(float(r["runtime_ms"]) for r in rows
                                             if r["status"] == "pass"),
            "passing_startup_estimate_ms": sum(float(r["startup_estimate_ms"]) for r in rows
                                              if r["status"] == "pass" and r["startup_estimate_ms"]),
            "passing_execution_residual_ms": sum(float(r["execution_residual_ms"]) for r in rows
                                                if r["status"] == "pass" and r["execution_residual_ms"]),
            "split_passing_count": sum(r["status"] == "pass" and bool(r["startup_estimate_ms"])
                                       for r in rows),
            "capped_startup_count": sum(r["split_status"] == "capped" for r in rows),
        }
        info["language_summaries"][language] = summary
        with (outdir / f"{language}.csv").open("w", newline="", encoding="utf-8") as handle:
            writer = csv.DictWriter(handle, FIELDS, lineterminator="\n")
            writer.writeheader()
            writer.writerows(rows)
        print(f"{language}: {passed}/{len(rows)} passing; child time {child_ms:.3f} [ms]; "
              f"language suite wall time {language_wall:.3f} [ms]", flush=True)
        if args.mode == "profile":
            if calibration and calibration["status"] == "measured":
                print(f"  startup no-op median {calibration['baseline_ms']:.3f} [ms] "
                      f"({len(calibration['sample_ms'])} independent probes); "
                      f"{summary['capped_startup_count']} clipped decompositions", flush=True)
            else:
                print(f"  startup calibration unavailable: {calibration['message']}", flush=True)
        for row in rows:
            if row["status"] != "pass":
                print(f"  {row['problem']:>2}: {row['status']}: "
                      f"{row['message'] or row['actual']}", flush=True)
    info["suite_wall_ms"] = (time.perf_counter_ns() - suite_start) / 1_000_000
    info["all_passed"] = failures == 0
    (outdir / "metadata.json").write_text(json.dumps(info, indent=2) + "\n", encoding="utf-8")
    # A pointer selects ONE run; never pool per-language CSVs from older runs.
    (base / "latest.json").write_text(json.dumps({"run_id": run_id}) + "\n", encoding="utf-8")
    print(f"Report: {outdir.relative_to(ROOT)}; failures: {failures}", flush=True)
    return int(failures != 0)


if __name__ == "__main__":
    raise SystemExit(main())
