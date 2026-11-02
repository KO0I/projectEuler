#!/usr/bin/env python3
"""Check pinned official fixtures offline, or explicitly restore them with --download."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import tempfile
import urllib.request

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "tests/data/manifest.json"


def valid(payload: bytes, item: dict) -> bool:
    return (len(payload) == item["bytes"] and
            hashlib.sha256(payload).hexdigest() == item["sha256"])


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--download", action="store_true",
                        help="restore missing or changed files from their official URLs")
    args = parser.parse_args()
    items = json.loads(MANIFEST.read_text(encoding="utf-8"))["files"]
    failures = []
    for item in items:
        path = ROOT / item["path"]
        if path.is_file() and valid(path.read_bytes(), item):
            continue
        if not args.download:
            failures.append(item["path"])
            continue
        with urllib.request.urlopen(item["url"], timeout=60) as response:
            payload = response.read()
        if not valid(payload, item):
            raise ValueError(f"Official download differs from the pinned input: {item['path']}")
        path.parent.mkdir(parents=True, exist_ok=True)
        # Validate before replacing a local file, and never leave partial input.
        with tempfile.NamedTemporaryFile(dir=path.parent, delete=False) as handle:
            temporary = Path(handle.name)
            handle.write(payload)
        try:
            os.replace(temporary, path)
        finally:
            temporary.unlink(missing_ok=True)
    if failures:
        for path in failures:
            print(f"Missing or changed official input: {path}")
        print("Run make fetch-inputs to restore the pinned files.")
        return 1
    print(f"Official input checksums: {len(items)}/{len(items)} pass")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
