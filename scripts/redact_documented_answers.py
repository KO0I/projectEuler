#!/usr/bin/env python3
"""Replace published Euler answers with suffix spoilers without changing test data."""

import csv
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = json.loads((ROOT / "tests/answers.json").read_text())
ANSWERS = {problem["answer"] for problem in MANIFEST["problems"]}


def spoiler(value: str, latex: bool = False) -> str:
    suffix = value[-4:]
    return f"\\texttt{{...{suffix}}}" if latex else f"…{suffix}"


def redact_value(value: object) -> object:
    if isinstance(value, str) and value in ANSWERS:
        return spoiler(value)
    return value


def redact_json(value: object, key: str | None = None) -> object:
    if isinstance(value, dict):
        return {name: redact_json(item, name) for name, item in value.items()}
    if isinstance(value, list):
        return [redact_json(item, key) for item in value]
    if key in {"expected", "actual", "answer"}:
        return redact_value(value)
    return value


def redact_csv(path: Path) -> None:
    with path.open(newline="") as source:
        reader = csv.DictReader(source)
        rows = list(reader)
        fields = reader.fieldnames
    assert fields is not None
    for row in rows:
        for field in ("expected", "actual", "answer"):
            if field in row:
                row[field] = redact_value(row[field])
    with path.open("w", newline="") as target:
        writer = csv.DictWriter(target, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def redact_tex() -> None:
    chapters = ROOT / "explanations/latex/chapters"
    for problem in MANIFEST["problems"]:
        path = chapters / f"{problem['stem']}.tex"
        if not path.exists():
            continue
        text = path.read_text()
        pattern = rf"(?<![.\d]){re.escape(problem['answer'])}(?!\d)"
        path.write_text(
            re.sub(pattern, lambda _match: spoiler(problem["answer"], latex=True), text)
        )


def main() -> None:
    for path in (ROOT / "docs/validation").rglob("*.csv"):
        redact_csv(path)
    for path in (ROOT / "docs/validation").rglob("*.json"):
        path.write_text(json.dumps(redact_json(json.loads(path.read_text())), indent=2) + "\n")
    redact_tex()


if __name__ == "__main__":
    main()
