#!/usr/bin/env python3
"""Structural coverage audit for Euler 1–120 (not a compiler or runtime test).

Asserts that every entrypoint exists, names a source-level native solver, the
Forth dispatcher covers every number, and no manifesto language claims refer
to missing executables. Runtime verification is deliberately separate.
"""
from __future__ import annotations

import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
NUMBERS = set(range(1, 121))
BASE = ROOT / 'solutions'
MANIFEST = ROOT / 'tests' / 'answers.json'


def numbered_names(folder: Path, extension: str) -> set[int]:
    found = set()
    for file in folder.glob(f'p*.{extension}'):
        if match := re.fullmatch(r'p(\d+)', file.stem):
            number = int(match.group(1))
            assert number not in found, f'duplicate {file}'
            found.add(number)
    return found


def audit() -> dict[str, int]:
    manifest = json.loads(MANIFEST.read_text(encoding='utf-8'))
    problems = manifest['problems']
    assert {item['number'] for item in problems} == NUMBERS, 'wrong problem numbers'
    assert len(problems) == 120, 'duplicate problems'
    result = {}
    for lang, folder, suffix in [
        ('rust', BASE / 'rust' / 'src' / 'bin', 'rs'),
        ('forth', BASE / 'forth', 'fs'),
        ('factor', BASE / 'factor', 'factor'),
    ]:
        available = numbered_names(folder, suffix)
        declared = {item['number'] for item in problems if lang in item['languages']}
        assert available == declared == NUMBERS, (
            f'{lang}: missing={sorted(NUMBERS - available)}, '
            f'extra={sorted(available - NUMBERS)}')
        result[lang] = len(available)

    forth = BASE / 'forth'
    dispatcher = (forth / 'solvers.fs').read_text()
    included = re.findall(r'^include (extra\d?\.fs)$', dispatcher, re.M)
    assert set(included) == {'extra.fs', 'extra2.fs', 'extra3.fs', 'extra4.fs'}
    sources = '\n'.join((forth / name).read_text() for name in
                        ('solvers.fs', 'extra.fs', 'extra2.fs', 'extra3.fs', 'extra4.fs'))
    definitions = [int(n) for n in re.findall(r'^:\s+p(\d+)\s', sources, re.M)]
    assert len(definitions) == 120 and set(definitions) == NUMBERS, 'Forth definitions mismatch'
    choices = [int(n) for n in re.findall(r'\b(\d+)\s+of\s+p\d+\s+endof', dispatcher)]
    assert len(choices) == 120 and set(choices) == NUMBERS, 'Forth switch mismatch'
    for n in NUMBERS:
        assert re.search(rf'\b{n}\s+of\s+p{n}\s+endof\b', dispatcher), n
        assert re.search(rf'\b{n}\s+euler-solve\s+\.\s+cr\s+bye',
                         (forth / f'p{n}.fs').read_text()), n

    factor = BASE / 'factor'
    originals = {1, 2, 5, 6, 9, 10, 11, 16, 20, 25, 28, 39}
    implemented = {}
    for path, vocab in [(factor / 'euler/extra/extra.factor', 'euler.extra'),
                        (factor / 'euler/advanced/advanced.factor', 'euler.advanced')]:
        assert f'IN: {vocab}' in path.read_text()
        for name in re.findall(r'^:{1,2}\s*(p\d+)\s*\(\s*--\s*n\s*\)',
                               path.read_text(), re.M):
            n = int(name[1:])
            assert n not in implemented and n not in originals, f'duplicate {name}'
            implemented[n] = vocab
    assert set(implemented) | originals == NUMBERS
    for n, vocab in implemented.items():
        text = (factor / f'p{n}.factor').read_text()
        assert f'USING: {vocab} io math.parser ;' in text
        assert f'p{n} number>string print' in text
    assert (factor / 'euler/shared/shared.factor').is_file(), 'missing shared Factor vocabulary'

    rust = BASE / 'rust' / 'src'
    def match_arms(path: Path) -> set[int]:
        source = path.read_text()
        start = source.index('pub fn solve(')
        return {int(n) for n in re.findall(r'\b(\d+)\s*=>', source[start:])}
    preexisting = match_arms(rust / 'lib.rs')
    additions = [match_arms(rust / 'extra' / f'{part}.rs')
                 for part in ('easy', 'middle', 'hard')]
    assert len(preexisting) == 37
    assert len(set.union(*additions)) == 83
    assert sum(map(len, additions)) == 83, 'duplicate Rust solver match arms'
    assert preexisting | set.union(*additions) == NUMBERS, 'Rust switch mismatch'
    assert 'pub mod extra_helpers;' in (rust / 'lib.rs').read_text()
    for n in NUMBERS:
        text = (rust / 'bin' / f'p{n}.rs').read_text()
        assert re.search(r'\bsolve\s*\(', text), f'Rust p{n}: no solver call'

    for item in problems:
        assert set(item['languages']) == {'c', 'rust', 'forth', 'factor'}, item['number']
    return result


if __name__ == '__main__':
    outcome = audit()
    print('Native source entrypoints: ' + ', '.join(f'{k} {v}/120' for k, v in outcome.items()))
    print('Structural audit only: compiler, runtime, and answer verification still required.')
