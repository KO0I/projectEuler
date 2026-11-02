#!/usr/bin/env python3
"""Structural coverage audit for every explicitly declared implementation.

Asserts that every entrypoint exists and C, Rust, and Forth algorithms belong
to their problem files. Libraries must not dispatch problem numbers or import
problem implementations. Runtime verification is deliberately separate.
"""
from __future__ import annotations

import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
FORTH_NUMBERS = set(range(1, 121))
RUST_NUMBERS = set(range(1, 256))
C_NUMBERS = set(range(1, 256))
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
    assert {item['number'] for item in problems} == C_NUMBERS, 'wrong problem numbers'
    assert len(problems) == 255, 'duplicate problems'
    result = {}
    for lang, folder, suffix in [
        ('rust', BASE / 'rust' / 'src' / 'bin', 'rs'),
        ('forth', BASE / 'forth', 'fs'),
        ('factor', BASE / 'factor', 'factor'),
    ]:
        available = numbered_names(folder, suffix)
        declared = {item['number'] for item in problems if lang in item['languages']}
        expected = (RUST_NUMBERS if lang == 'rust' else FORTH_NUMBERS
                    if lang == 'forth' else available)
        assert available == declared == expected, (
            f'{lang}: declared/source mismatch; '
            f'undeclared={sorted(available - declared)}, missing={sorted(declared - available)}')
        result[lang] = len(available)

    forth = BASE / 'forth'
    for n in FORTH_NUMBERS:
        text = (forth / f'p{n}.fs').read_text()
        assert re.findall(r'^:\s+p(\d+)\s', text, re.M) == [str(n)], f'Forth p{n}: wrong owner'
        assert 'euler-solve' not in text, f'Forth p{n}: central dispatcher'
        assert not re.search(r'^(?:include|require)\s+(?:p\d+|solvers|extra\d*)\.fs', text, re.M), n
        assert re.search(rf'\bp{n}\s+\.\s+cr\s+bye', text), f'Forth p{n}: no direct invocation'
    for helper in ('math.fs', 'big.fs'):
        text = (forth / helper).read_text()
        assert not re.search(r'^:\s+(?:p\d+|euler-solve)\s', text, re.M), helper
        assert not re.search(r'^(?:include|require)\s+p\d+\.fs', text, re.M), helper

    factor = BASE / 'factor'
    originals = {1, 2, 5, 6, 9, 10, 11, 16, 20, 25, 28, 39}
    implemented = {}
    for path, vocab in [(factor / 'euler/extra/extra.factor', 'euler.extra'),
                        (factor / 'euler/advanced/advanced.factor', 'euler.advanced'),
                        (factor / 'euler/extended/extended.factor', 'euler.extended')]:
        assert f'IN: {vocab}' in path.read_text()
        for name in re.findall(r'^:{1,2}\s*(p\d+)\s*\([^)]*\)',
                               path.read_text(), re.M):
            n = int(name[1:])
            assert n not in implemented and n not in originals, f'duplicate {name}'
            implemented[n] = vocab
    assert set(range(1, 121)) <= set(implemented) | originals
    for n, vocab in implemented.items():
        text = (factor / f'p{n}.factor').read_text()
        assert vocab in text, f'Factor p{n}: wrong vocabulary owner'
        assert re.search(rf'\bp{n}\b', text), f'Factor p{n}: no direct invocation'
    for n in numbered_names(factor, 'factor') - set(implemented) - originals:
        text = (factor / f'p{n}.factor').read_text()
        assert f'project-euler.{n}' in text, f'Factor p{n}: no native vocabulary'
        assert f'euler{n}' in text, f'Factor p{n}: no native invocation'
    assert (factor / 'euler/shared/shared.factor').is_file(), 'missing shared Factor vocabulary'

    rust = BASE / 'rust' / 'src'
    for n in RUST_NUMBERS:
        text = (rust / 'bin' / f'p{n}.rs').read_text()
        assert re.search(r'\bfn solve\s*\(\s*\)\s*->', text), f'Rust p{n}: no local algorithm'
        assert 'project_euler::solve' not in text, f'Rust p{n}: central dispatcher'
        assert not re.search(r'include!\s*\(|#\[path|^\s*mod p\d+', text, re.M), f'Rust p{n}: external solver'
    for helper in rust.rglob('*.rs'):
        if 'bin' in helper.relative_to(rust).parts:
            continue
        text = helper.read_text()
        assert not re.search(r'\bfn (?:solve|p\d+)\s*\(', text), f'Rust library contains problem solver: {helper}'

    for item in problems:
        path = BASE / 'c' / (item['stem'] + '.c')
        text = path.read_text()
        assert 'euler_solve' not in text, f'C {item["number"]}: central dispatcher'
        assert re.search(r'\bint main\s*\(', text), f'C {item["number"]}: no main'
    for helper in (ROOT / 'support/c').glob('euler_*.*'):
        text = helper.read_text()
        assert not re.search(r'\b(?:euler_solve|problem\d+)\s*\(', text), helper

    factor_numbers = numbered_names(factor, 'factor')
    for item in problems:
        n = item['number']
        expected_languages = {'c', 'rust'}
        if n <= 120:
            expected_languages.add('forth')
        if n in factor_numbers:
            expected_languages.add('factor')
        assert set(item['languages']) == expected_languages, item['number']
    return result


if __name__ == '__main__':
    outcome = audit()
    print('Native source entrypoints: ' + ', '.join(f'{k} {v}' for k, v in outcome.items()))
    print('Structural audit only: compiler, runtime, and answer verification still required.')
