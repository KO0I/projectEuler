#!/usr/bin/env python3
"""Require exactly one C solution per Euler ID 1..255, preserving report exclusions."""
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    manifest = json.loads((ROOT / "tests/answers.json").read_text())
    problems = manifest["problems"]
    expected = set(range(1, 256))
    numbers = [p["number"] for p in problems]
    if len(numbers) != 255 or set(numbers) != expected:
        raise ValueError("The manifest must contain every ID from 1 to 255 exactly once")
    report = (ROOT / "explanations/latex/project_euler_c_explanations.tex").read_text()
    covered = {int(n) for n in re.findall(r"\\input\{chapters/[^{}]+_(\d+)\.tex\}", report)}
    marked = {p["number"] for p in problems if p.get("covered_by_report")}
    if covered != marked or len(covered) != 37:
        raise ValueError("Manifest exclusions disagree with the existing 37 report chapters")
    sources = list((ROOT / "solutions/c").glob("*.c"))
    if {p.stem for p in sources} != {p["stem"] for p in problems}:
        raise ValueError("C source filenames and the manifest do not match exactly")
    for item in problems:
        if not item["stem"].endswith(f"_{item['number']}"):
            raise ValueError(f"Problem number must end its filename: {item['stem']}")
        answer = str(item["answer"])
        if not answer or "\n" in answer or "\r" in answer:
            raise ValueError(f"Invalid exact-string reference answer for {item['number']}")
        if "c" not in item["languages"]:
            raise ValueError(f"Missing C coverage for {item['number']}")
    print(f"C coverage: 255/255; {len(covered)} report solutions retained, 218 added")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
