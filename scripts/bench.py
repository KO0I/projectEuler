#!/usr/bin/env python3
"""Run each solution exactly once; record correctness and wall time.

Wall time includes interpreter/process startup and excludes compilation.
This is single-run measurement: neither error bars nor significance claims
are warranted. Failures and timeouts retain their measured durations.
"""
from __future__ import annotations
import argparse
import csv
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import platform
import re
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "tests" / "answers.json"
LANGUAGES = ("c", "rust", "forth")
FIELDS = ("language", "problem", "stem", "runtime_ms", "status",
          "expected", "actual", "exit_code", "message")


def command(language: str, number: int, stem: str) -> list[str]:
    if language == "c":
        return [str(ROOT / "build" / "c" / stem)]
    if language == "rust":
        suffix = ".exe" if os.name == "nt" else ""
        return [str(ROOT / "solutions" / "rust" / "target" /
                    "release" / (f"p{number}" + suffix))]
    if language == "forth":
        return ["gforth", str(ROOT / "solutions" / "forth" / f"p{number}.fs")]
    raise ValueError(language)


def tool_version(argv: list[str]) -> str:
    try:
        p = subprocess.run(argv, text=True, stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT, timeout=4)
        return p.stdout.strip().splitlines()[0][:200]
    except (OSError, subprocess.TimeoutExpired, IndexError):
        return "unavailable"


def run_one(language: str, problem: dict, timeout: float) -> dict:
    number, stem, expected = (problem[k] for k in ("number", "stem", "answer"))
    cmd = command(language, number, stem)
    row = dict(language=language, problem=number, stem=stem,
               runtime_ms="", status="missing", expected=expected,
               actual="", exit_code="", message="")
    if not shutil.which(cmd[0]) and not Path(cmd[0]).is_file():
        row["message"] = "executable/interpreter not found"
        return row
    start = time.perf_counter_ns()
    try:
        proc = subprocess.run(cmd, cwd=ROOT, text=True, capture_output=True,
                              timeout=timeout, check=False)
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
        elif stdout != expected:
            row["status"] = "wrong-answer"
        else:
            row["status"] = "pass"
    except subprocess.TimeoutExpired:
        elapsed = time.perf_counter_ns() - start
        row["runtime_ms"] = f"{elapsed / 1_000_000:.6f}"
        row["status"] = "timeout"
        row["message"] = f"exceeded {timeout:g} [s]"
    except OSError as exc:
        row["status"] = "error"
        row["message"] = str(exc)[:500]
    return row


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--languages", nargs="+", choices=LANGUAGES,
                        default=list(LANGUAGES))
    parser.add_argument("--mode", choices=("test", "profile"), default="test")
    parser.add_argument("--timeout", type=float, default=30.0)
    args = parser.parse_args()
    problems = json.loads(MANIFEST.read_text(encoding="utf-8"))["problems"]
    outdir = ROOT / "build" / "profile"
    outdir.mkdir(parents=True, exist_ok=True)
    report = []
    suite_start = time.perf_counter_ns()
    for language in args.languages:
        rows = [run_one(language, item, args.timeout) for item in problems]
        report.extend(rows)
        with (outdir / f"{language}.csv").open("w", newline="", encoding="utf-8") as handle:
            writer = csv.DictWriter(handle, FIELDS)
            writer.writeheader()
            writer.writerows(rows)
        successes = sum(row["status"] == "pass" for row in rows)
        measured = sum(float(row["runtime_ms"]) for row in rows if row["runtime_ms"])
        print(f"{language}: {successes}/{len(rows)} passing; sum of measured child runtimes = "
              f"{measured:.3f} [ms]")
        for row in rows:
            if row["status"] != "pass":
                print(f"  {row['problem']:>2}: {row['status']}: {row['message'] or row['actual']}")
    elapsed = (time.perf_counter_ns() - suite_start) / 1_000_000
    info = {"timestamp_utc": datetime.now(timezone.utc).isoformat(),
            "clock": "time.perf_counter_ns",
            "runs_per_solution": 1,
            "warmup_runs": 0,
            "compilation_included": False,
            "startup_included": True,
            "unit": "ms",
            "timeout_s": args.timeout,
            "languages": args.languages,
            "suite_wall_ms": elapsed,
            "sum_of_individual_times_ms": {
                lang: sum(float(r["runtime_ms"]) for r in report
                          if r["language"] == lang and r["runtime_ms"])
                for lang in args.languages},
            "environment": {"os": platform.platform(),
                            "python": sys.version,
                            "machine": platform.machine(),
                            "cc": tool_version(["gcc", "--version"]),
                            "rustc": tool_version(["rustc", "--version"]),
                            "gforth": tool_version(["gforth", "--version"])}}
    (outdir / "metadata.json").write_text(json.dumps(info, indent=2) + "\n",
                                           encoding="utf-8")
    failures = sum(row["status"] != "pass" for row in report)
    print(f"suite wall time: {elapsed:.3f} [ms]; failures: {failures}")
    return int(failures != 0)


if __name__ == "__main__":
    raise SystemExit(main())
