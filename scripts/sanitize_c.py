#!/usr/bin/env python3
"""Build and validate every C answer with AddressSanitizer and UBSan on Linux."""
from __future__ import annotations

import json
import os
from pathlib import Path
import shlex
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "build/sanitize"
FLAGS = ["-O1", "-g", "-std=c11", "-Wall", "-Wextra", "-Wpedantic", "-Werror",
         "-fno-omit-frame-pointer", "-fsanitize=address,undefined", "-fno-pie", "-no-pie",
         "-Isupport/c"]


def source_revision() -> str | None:
    # A downloaded source archive has no .git directory and still runs the checks.
    try:
        result = subprocess.run(["git", "rev-parse", "HEAD"], cwd=ROOT,
                                capture_output=True, text=True, check=False)
        return result.stdout.strip() if result.returncode == 0 else None
    except OSError:
        return None


def main() -> int:
    if not sys.platform.startswith("linux"):
        raise SystemExit("This sanitizer target requires Linux and GCC-compatible sanitizers")
    compiler = shlex.split(os.environ.get("CC", "gcc"))
    OUT.mkdir(parents=True, exist_ok=True)
    common = OUT / "euler_common.o"
    subprocess.run(compiler + FLAGS + ["-c", "support/c/euler_common.c", "-o", str(common)],
                   cwd=ROOT, check=True)
    problems = json.loads((ROOT / "tests/answers.json").read_text())["problems"]
    env = os.environ.copy()
    env["ASAN_OPTIONS"] = "detect_leaks=1:halt_on_error=1"
    env["UBSAN_OPTIONS"] = "halt_on_error=1:print_stacktrace=1"
    results = []
    for problem in problems:
        binary = OUT / problem["stem"]
        source = f"solutions/c/{problem['stem']}.c"
        subprocess.run(compiler + FLAGS + [source, str(common), "-o", str(binary), "-lgmp", "-lm"],
                       cwd=ROOT, check=True)
        result = subprocess.run([str(binary)], cwd=ROOT, env=env, capture_output=True,
                                text=True, timeout=90, check=False)
        passed = result.returncode == 0 and result.stdout.strip() == problem["answer"]
        row = {"number": problem["number"], "passed": passed, "exit_code": result.returncode,
               "expected": problem["answer"], "actual": result.stdout.strip(),
               "stderr": result.stderr}
        results.append(row)
        if not passed:
            print(json.dumps(row, indent=2), flush=True)
        if len(results) % 20 == 0:
            print(f"Sanitized answers: {len(results)}/{len(problems)} checked", flush=True)
    report = {"compiler": compiler, "flags": FLAGS,
              "sanitizer_options": {name: env[name] for name in ("ASAN_OPTIONS", "UBSAN_OPTIONS")},
              "source_revision": source_revision(),
              "results": results}
    (OUT / "results.json").write_text(json.dumps(report, indent=2) + "\n")
    failures = sum(not row["passed"] for row in results)
    print(f"AddressSanitizer/UBSan: {len(results) - failures}/{len(results)} answers pass")
    return int(failures != 0)


if __name__ == "__main__":
    raise SystemExit(main())
