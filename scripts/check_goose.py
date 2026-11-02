#!/usr/bin/env python3
"""Audit exact Goose coverage and keep algorithms in their problem files."""
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]

def audit():
    problems = json.loads((ROOT / 'tests/answers.json').read_text())['problems']
    declared = [p for p in problems if 'goose' in p['languages']]
    assert {p['number'] for p in declared} == set(range(1, 101))
    expected = {p['stem'] + '.goose' for p in declared}
    files = list((ROOT / 'solutions/goose').glob('*.goose'))
    assert {p.name for p in files} == expected, 'Goose source/manifest mismatch'
    for path in files:
        text = path.read_text()
        assert re.search(r'fn solve\(\) -> i64', text), f'{path}: missing owned solver'
        assert 'print(solve());' in text, f'{path}: missing direct invocation'
        assert 'extern fn' not in text, f'{path}: C wrapper rather than native solver'
        assert 'import euler from "../../support/goose";' in text
    helper = (ROOT / 'support/goose/euler.goose').read_text()
    assert not re.search(r'fn (?:solve|p\d+)\b', helper), 'shared problem dispatcher'
    return len(files)

if __name__ == '__main__':
    print(f'Goose: {audit()}/100 problem-owned implementations')
