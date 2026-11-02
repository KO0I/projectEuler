#!/usr/bin/env python3
"""Refresh Project Euler metadata and generate explicitly incomplete scaffolds.

Ordinary generation is offline and reads docs/project_euler_catalog.json.  Network
access occurs only with --refresh, which is intended as a deliberate maintainer
operation when Project Euler publishes new problems.
"""
from __future__ import annotations

import argparse
from datetime import date
from html import unescape
import json
from pathlib import Path
import re
from urllib.request import Request, urlopen

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / "docs" / "project_euler_catalog.json"
SCAFFOLDS = ROOT / "scaffolds"
LANGUAGES = ("c", "rust", "forth", "factor")
ENTRY_RE = re.compile(
    r'<td class="id_column">(\d+)</td><td><a href="problem=\d+"[^>]*>(.*?)</a>'
)


def fetch(url: str) -> str:
    request = Request(url, headers={"User-Agent": "ProjectEuler-multilang catalog refresh"})
    with urlopen(request, timeout=30) as response:
        return response.read().decode("utf-8")


def parse_entries(page: str) -> dict[int, str]:
    return {int(number): unescape(re.sub(r"<[^>]+>", "", title))
            for number, title in ENTRY_RE.findall(page)}


def refresh_catalog() -> dict:
    entries: dict[int, str] = {}
    first = fetch("https://projecteuler.net/archives")
    entries.update(parse_entries(first))
    pages = [int(value) for value in re.findall(r"archives;page=(\d+)", first)]
    for page in range(2, max(pages, default=1) + 1):
        entries.update(parse_entries(fetch(f"https://projecteuler.net/archives;page={page}")))
    entries.update(parse_entries(fetch("https://projecteuler.net/recent")))
    maximum = max(entries)
    expected = set(range(1, maximum + 1))
    if set(entries) != expected:
        missing = sorted(expected - set(entries))
        raise RuntimeError(f"official catalog is not contiguous; missing {missing}")
    catalog = {
        "schema": 1,
        "snapshot_date": date.today().isoformat(),
        "source": "https://projecteuler.net/archives",
        "recent_source": "https://projecteuler.net/recent",
        "maximum_problem": maximum,
        "problems": [
            {"number": number, "title": entries[number],
             "url": f"https://projecteuler.net/problem={number}"}
            for number in sorted(entries)
        ],
    }
    CATALOG.write_text(json.dumps(catalog, indent=2, ensure_ascii=False) + "\n",
                       encoding="utf-8")
    return catalog


def slug(title: str) -> str:
    value = title.lower().replace("$", "")
    value = re.sub(r"[^a-z0-9]+", "_", value).strip("_")
    return value or "problem"


def path_for(language: str, problem: dict) -> Path:
    number = problem["number"]
    if language == "c":
        return SCAFFOLDS / language / f"{slug(problem['title'])}_{number}.c"
    extension = {"rust": "rs", "forth": "fs", "factor": "factor"}[language]
    return SCAFFOLDS / language / f"p{number}.{extension}"


def content_for(language: str, problem: dict) -> str:
    number, title, url = problem["number"], problem["title"], problem["url"]
    diagnostic = f"Project Euler {number} scaffold: not implemented"
    if language == "c":
        return (f"/* {title}: {url}\n * Solution scaffold; excluded from coverage. */\n"
                "#include <stdio.h>\n#include <stdlib.h>\n\n"
                f"int main(void) {{\n    fputs(\"{diagnostic}\\n\", stderr);\n"
                "    return EXIT_FAILURE;\n}\n")
    if language == "rust":
        return (f"//! {title}: {url}\n//! Solution scaffold; excluded from coverage.\n\n"
                f"fn main() {{\n    eprintln!(\"{diagnostic}\");\n"
                "    std::process::exit(1);\n}\n")
    if language == "forth":
        return (f"\\ {title}: {url}\n\\ Solution scaffold; excluded from coverage.\n\n"
                f'.( {diagnostic} ) cr\n1 (bye)\n')
    return (f"! {title}: {url}\n! Solution scaffold; excluded from coverage.\n"
            "USING: io kernel ;\nIN: scratchpad\n"
            f'\"{diagnostic}\" print\n\"not-implemented\" throw\n')


def expected_scaffolds(catalog: dict) -> dict[Path, str]:
    """Return scaffolds only where the language has no owned solution.

    The solved frontier is intentionally allowed to be non-contiguous: Factor
    has bounded, documented exceptions in 121..255, whereas Rust now covers
    that range completely.  Looking at the canonical solution path keeps a
    newly implemented problem from retaining a contradictory failure stub.
    """
    expected: dict[Path, str] = {}
    for problem in catalog["problems"]:
        number = problem["number"]
        languages = LANGUAGES if number >= 256 else LANGUAGES[1:] if number >= 121 else ()
        for language in languages:
            solution = (ROOT / "solutions" / language /
                        (f"{slug(problem['title'])}_{number}.c" if language == "c" else
                         f"p{number}.rs" if language == "rust" else
                         f"p{number}.fs" if language == "forth" else
                         f"p{number}.factor"))
            if language == "rust":
                solution = ROOT / "solutions/rust/src/bin" / f"p{number}.rs"
            if solution.is_file():
                continue
            expected[path_for(language, problem)] = content_for(language, problem)
    return expected


def generate(catalog: dict, check: bool) -> int:
    expected = expected_scaffolds(catalog)
    mismatches = []
    for path, content in expected.items():
        if check:
            if not path.is_file() or path.read_text(encoding="utf-8") != content:
                mismatches.append(path.relative_to(ROOT))
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content, encoding="utf-8")
    actual = {path for language in LANGUAGES
              for path in (SCAFFOLDS / language).glob("*") if path.is_file()}
    extras = sorted(actual - set(expected))
    if extras:
        mismatches.extend(path.relative_to(ROOT) for path in extras)
    if mismatches:
        print("Scaffold mismatch: " + ", ".join(map(str, mismatches[:20])))
        return 1
    counts = {language: sum(path.parent.name == language for path in expected)
              for language in LANGUAGES}
    print("Scaffolds: " + ", ".join(f"{key}={value}" for key, value in counts.items())
          + f", total={len(expected)}")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--refresh", action="store_true",
                        help="refresh the versioned catalog from projecteuler.net")
    parser.add_argument("--check", action="store_true",
                        help="verify generated files without changing them")
    args = parser.parse_args()
    if args.refresh and args.check:
        parser.error("--refresh and --check are mutually exclusive")
    catalog = refresh_catalog() if args.refresh else json.loads(CATALOG.read_text())
    maximum = catalog["maximum_problem"]
    numbers = [item["number"] for item in catalog["problems"]]
    if numbers != list(range(1, maximum + 1)):
        raise ValueError("catalog problem IDs must be contiguous and ordered")
    return generate(catalog, args.check)


if __name__ == "__main__":
    raise SystemExit(main())
