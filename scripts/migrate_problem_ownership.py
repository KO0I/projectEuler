#!/usr/bin/env python3
"""One-time, reviewable extraction of the old C/Rust/Forth dispatchers.

Run on the pre-migration source tree. Each algorithm body is copied verbatim;
dependency closure assigns single-problem helpers to their owner and keeps
parameterized helpers used by multiple problems in the mathematical library.
Use --write to apply. Subsequent runs detect the completed migration and exit.
"""
from __future__ import annotations

import argparse
from collections import Counter
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]


def mask(source):
    """Hide comments and literals without changing offsets or line breaks."""
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\\n])\''
    return re.sub(pattern, lambda m: re.sub(r'[^\n]', ' ', m[0]), source)


def end_brace(source, start):
    clean = mask(source)
    depth = 0
    for index in range(start, len(clean)):
        if clean[index] == '{':
            depth += 1
        elif clean[index] == '}':
            depth -= 1
            if depth == 0:
                return index + 1
    raise ValueError('Unbalanced braces')


def identifiers(source):
    return set(re.findall(r'\b[A-Za-z_]\w*\b', mask(source)))


def closure(source, definitions, tokens=identifiers):
    needed = set()
    pending = tokens(source) & definitions.keys()
    while pending:
        name = pending.pop()
        if name in needed:
            continue
        needed.add(name)
        pending |= tokens(definitions[name]) & definitions.keys() - needed
    return needed


def c_extract(problems):
    path = ROOT / 'support/c/euler_common.c'
    source = path.read_text()
    functions = {}
    for match in re.finditer(r'^static [^\n{]+?\b(\w+)\([^\n{]*\)\s*\{', source, re.M):
        functions[match[1]] = source[match.start():end_brace(source, match.end()-1)]
    switch = source[source.index('long long euler_solve'):]
    matches = list(re.finditer(r'\bcase (\d+):', switch))
    cases = {}
    for index, match in enumerate(matches):
        stop = matches[index+1].start() if index+1 < len(matches) else switch.index('default:')
        body = switch[match.end():stop].strip()
        if body.startswith('{'):
            assert end_brace(body, 0) == len(body)
            body = body[1:-1]
        cases[int(match[1])] = body
    assert len(cases) == 37
    needs = {n: closure(body, functions) for n, body in cases.items()}
    users = Counter(name for names in needs.values() for name in names)
    # Reusable operations remain exposed for helper regression tests too.
    common = {name for name in functions if not re.fullmatch(r'problem\d+', name)}
    common -= {'number_letters', 'leap'}
    assert all(users[name] <= 1 for name in functions.keys() - common)
    includes = '\n'.join(re.findall(r'^#include .+$', source, re.M))
    out = {}
    for n, body in cases.items():
        item = problems[n]
        local = '\n\n'.join(text for name, text in functions.items() if name in needs[n] - common)
        direct = re.fullmatch(r'return (problem\d+)\(\);', body)
        if direct:
            text = functions[direct[1]]
            body = text[text.index('{')+1:-1]
            local = '\n\n'.join(text for name, text in functions.items()
                                if name in needs[n] - common and name != direct[1])
        out[f'solutions/c/{item["stem"]}.c'] = (
            f'/* Project Euler {n}: {item["title"]}.\n * This file owns the problem algorithm. */\n'
            + includes + '\n\n' + (local + '\n\n' if local else '')
            + 'static long long solve(void) {\n' + body.strip() + '\n}\n\n'
            + 'int main(void) {\n    long long answer = solve();\n'
            + ('    if (answer < 0) return 1;\n' if n != 27 else '')
            + '    printf("%lld\\n", answer);\n    return 0;\n}\n')
    declarations = []
    bodies = []
    for name, text in functions.items():
        if name in common:
            text = text.removeprefix('static ')
            declarations.append(text[:text.index('{')].strip() + ';')
            bodies.append(text)
    out['support/c/euler_common.h'] = ('#ifndef EULER_COMMON_H\n#define EULER_COMMON_H\n'
        '#include <stdint.h>\n#include <gmp.h>\n\n/* Reusable mathematics only; no problem dispatch. */\n'
        + '\n'.join(declarations) + '\n#endif\n')
    out['support/c/euler_common.c'] = ('/* Reusable mathematics for the individual C programs. */\n'
        + includes + '\n\n' + '\n\n'.join(bodies) + '\n')
    return out, []


def rust_items(source):
    result = {}
    for match in re.finditer(r'^(?:#\[[^\n]*?\]\s*)?(fn|struct|enum|const|type|impl)\s+(\w+)', source, re.M):
        name = match[2]
        if match[1] in ('const', 'type'):
            clean = mask(source)
            depth = 0
            for index in range(match.end(), len(clean)):
                char = clean[index]
                if char in '([{':
                    depth += 1
                elif char in ')]}':
                    depth -= 1
                elif char == ';' and depth == 0:
                    end = index + 1
                    break
        else:
            start = source.index('{', match.end())
            end = end_brace(source, start)
        result[name] = result.get(name, '') + source[match.start():end] + '\n'
    return result


def rust_arms(source):
    start = source.index('match n {', source.index('pub fn solve')) + len('match n {')
    end = end_brace(source, start-1)-1
    body = source[start:end]
    clean = mask(body)
    depths = []
    depth = 0
    for char in clean:
        depths.append(depth)
        if char in '({[':
            depth += 1
        elif char in ')}]':
            depth -= 1
    matches = [m for m in re.finditer(r'\b(\d+|_)\s*=>', clean) if depths[m.start()] == 0]
    arms = {}
    for index, match in enumerate(matches):
        if match[1] == '_':
            continue
        stop = matches[index+1].start() if index+1 < len(matches) else len(body)
        arms[int(match[1])] = body[match.end():stop].strip().removesuffix(',')
    return arms


def rust_extract(problems):
    base = ROOT / 'solutions/rust/src'
    original = (base / 'lib.rs').read_text()
    helpers = rust_items(original[:original.index('pub fn solve')])
    existing = (base / 'extra_helpers.rs').read_text()
    # These helpers already exist with the same signatures in extra_helpers.
    for name in ('prime', 'sieve', 'gcd'):
        del helpers[name]
    helpers['digit_sum'] = 'fn digit_sum(n: &BigUint) -> i64 { digits_sum_big(n) as i64 }\n'
    helpers['is_palindrome'] = 'fn is_palindrome(n: u32) -> bool { palindrome(n as u64, 10) }\n'
    reciprocal = rust_items((base / 'extra/middle.rs').read_text())['p108_110']
    reciprocal = reciprocal.replace('p108_110', 'least_reciprocal_denominator')
    shared = {name: text for name, text in helpers.items() if name not in ('letters', 'leap')}
    shared['least_reciprocal_denominator'] = reciprocal
    existing += '\n' + '\n'.join('pub ' + text for text in shared.values())
    ext_names = set(re.findall(r'^pub fn (\w+)', existing, re.M))
    out = {'solutions/rust/src/extra_helpers.rs': existing}
    seen = set()
    files = ['lib.rs', 'extra/easy.rs', 'extra/middle.rs', 'extra/hard.rs']
    for file in files:
        source = (base / file).read_text()
        definitions = rust_items(source[:source.index('pub fn solve')])
        if file == 'lib.rs':
            definitions = {name: text for name, text in definitions.items() if name in ('letters', 'leap')}
        definitions.pop('p108_110', None)
        for n, arm in rust_arms(source).items():
            assert n not in seen, n
            seen.add(n)
            arm = arm.replace('p108_110', 'least_reciprocal_denominator')
            needs = closure(arm, definitions)
            local = '\n'.join(text for name, text in definitions.items() if name in needs)
            # A dedicated existing pN function becomes the local solve function.
            if arm == f'p{n}()':
                local = re.sub(rf'\bfn p{n}\(', 'fn solve(', local)
                body = local
            else:
                body = local + '\nfn solve() -> i64 {\n' + arm + '\n}\n'
            if file == 'lib.rs':
                body = body.replace('../../../', '../../../../')
            names = identifiers(body)
            imports = []
            if names & ext_names:
                imports.append('use project_euler::extra_helpers::{' + ', '.join(sorted(names & ext_names)) + '};')
            if 'BigUint' in names:
                imports.append('use num_bigint::BigUint;')
            traits = [trait for method, trait in [('one', 'One'), ('zero', 'Zero')] if method in names]
            if traits:
                imports.append('use num_traits::{' + ', '.join(traits) + '};')
            collections = names & {'HashMap', 'HashSet'}
            if collections:
                imports.append('use std::collections::{' + ', '.join(sorted(collections)) + '};')
            out[f'solutions/rust/src/bin/p{n}.rs'] = (
                f'//! Project Euler {n}: {problems[n]["title"]}.\n'
                + '\n'.join(imports) + '\n\n' + body
                + '\nfn main() {\n    println!("{}", solve());\n}\n')
    assert seen == set(problems)
    out['solutions/rust/src/lib.rs'] = (
        '//! Reusable mathematical and input operations. Each binary owns its problem algorithm.\n'
        'pub mod extra_helpers;\n')
    return out, ['solutions/rust/src/' + name for name in files[1:]] + ['solutions/rust/src/extra.rs']


def forth_tokens(source):
    source = re.sub(r'\\[^\n]*|\([^)]*\)|(?:s|abort|\.)"[^"\n]*"', ' ', source)
    locals_used = set(' '.join(re.findall(r'\{([^}]*)\}', source)).split())
    return set(source.split()) - locals_used


def forth_extract(problems):
    files = ['solvers.fs', 'extra.fs', 'extra2.fs', 'extra3.fs', 'extra4.fs']
    blocks = []
    for file in files:
        source = (ROOT / 'solutions/forth' / file).read_text()
        if file == 'solvers.fs':
            source = source[:source.index('include extra.fs')]
        matches = list(re.finditer(r'^(?::\s+\S+|:noname\b|create\s+\S+|f?variable\s+\S+|defer\s+\S+)', source, re.M))
        for index, match in enumerate(matches):
            end = matches[index+1].start() if index+1 < len(matches) else len(source)
            text = source[match.start():end].strip() + '\n'
            if text.startswith(':noname'):
                owner = re.search(r'; is (\S+)', text)[1]
                blocks.append(([owner + '-implementation'], text))
            else:
                names = re.findall(r'(?:^|\s)(?::|create|f?variable|defer)\s+(\S+)', match[0])
                # There are two adjacent variable declarations on one source line.
                if text.startswith('variable'):
                    names = re.findall(r'\bvariable\s+(\S+)', text.splitlines()[0])
                blocks.append((names, text))
    # Give the shared parameterized reciprocal search a mathematical name.
    blocks = [(names, re.sub(r'\bp108-(\w+)', r'reciprocal-\1', text)) for names, text in blocks]
    blocks = [([re.sub(r'^p108-', 'reciprocal-', name) for name in names], text) for names, text in blocks]
    definitions = {name: text for names, text in blocks for name in names}
    definitions['p93-expression'] += definitions['p93-expression-implementation']
    needs = {n: closure(f'p{n}', definitions, forth_tokens) for n in problems}
    counts = Counter(name for names in needs.values() for name in names)
    shared = {name for name, count in counts.items() if count > 1}
    # Generic operations exercised by the helper tests also remain reusable.
    test_helpers = {'require-64bit', 'is-prime?', 'integer-sqrt', 'digit-pow-sum',
                    'divisor-count', 'proper-divisor-sum', 'pentagonal?',
                    'prefix@', 'prefix!', 'multiply-mod', 'power-mod'}
    shared |= closure(' '.join(test_helpers), definitions, forth_tokens)
    assert not any(re.fullmatch(r'p\d+', name) for name in shared)
    def render(selected):
        return '\n'.join(text for names, text in blocks
                         if set(names) & selected or
                         (names == ['p93-expression-implementation'] and 'p93-expression' in selected))
    out = {'solutions/forth/math.fs': '\\ Reusable mathematics and input; no problem dispatch.\nrequire big.fs\n\n' + render(shared)}
    for n in problems:
        out[f'solutions/forth/p{n}.fs'] = (
            f'\\ Project Euler {n}: {problems[n]["title"]}.\nrequire math.fs\n\n'
            + render(needs[n] - shared)
            + f'\n[undefined] euler-testing [if] p{n} . cr bye [then]\n')
    return out, ['solutions/forth/' + name for name in files]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write', action='store_true')
    args = parser.parse_args()
    if not (ROOT / 'solutions/forth/solvers.fs').exists():
        print('Dispatchers already extracted; no changes made.')
        return
    problems = {p['number']: p for p in json.loads((ROOT / 'tests/answers.json').read_text())['problems']}
    outputs, obsolete = {}, []
    for extract in (c_extract, rust_extract, forth_extract):
        files, removed = extract(problems)
        outputs.update(files)
        obsolete.extend(removed)
    print(f'Extracted algorithms into {len(outputs)} files; retire {len(obsolete)} dispatcher files.')
    if args.write:
        for path, source in outputs.items():
            (ROOT / path).write_text(source)
        for path in obsolete:
            (ROOT / path).unlink()


if __name__ == '__main__':
    main()
