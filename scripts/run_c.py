#!/usr/bin/env python3
"""Build and run one C solution by official problem number, from any directory."""
import argparse
import json
from pathlib import Path
import subprocess
import os

ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("number", type=int, choices=range(1, 256), metavar="1..255")
    args = parser.parse_args()
    manifest = json.loads((ROOT / "tests/answers.json").read_text())
    problem = next(p for p in manifest["problems"] if p["number"] == args.number)
    target = "build/c/" + problem["stem"]
    subprocess.run(["make", target], cwd=ROOT, check=True)
    executable = ROOT / (target + (".exe" if os.name == "nt" else ""))
    return subprocess.run([str(executable)], cwd=ROOT, check=False).returncode


if __name__ == "__main__":
    raise SystemExit(main())
