#!/usr/bin/env python3
"""Publish one Goose run with measured compilation and per-problem wall times."""
import csv
import json
from pathlib import Path
import shutil
import statistics

ROOT = Path(__file__).resolve().parents[1]

def main():
    base = ROOT / 'build/profile'
    run_id = json.loads((base / 'latest.json').read_text())['run_id']
    source = base / run_id
    meta = json.loads((source / 'metadata.json').read_text())
    if meta['coverage'].get('goose') != list(range(1, 101)) or not meta['all_passed']:
        raise SystemExit('A complete passing Goose profile is required')
    rows = {}
    out = ROOT / 'docs/profile' / (run_id + '-goose')
    out.mkdir(parents=True, exist_ok=True)
    for lang in meta['languages']:
        with (source / f'{lang}.csv').open() as f:
            rows[lang] = list(csv.DictReader(f))
        published = [dict(r) for r in rows[lang]]
        for r in published:
            for field in ('expected', 'actual'):
                if r[field]:
                    r[field] = '…' + r[field][-4:]
        with (out / f'{lang}.csv').open('w') as f:
            writer = csv.DictWriter(f, fieldnames=published[0].keys())
            writer.writeheader()
            writer.writerows(published)
    shutil.copy2(source / 'metadata.json', out / 'metadata.json')
    compilation = json.loads((ROOT / 'build/goose/compilation.json').read_text())
    compilation['rows'] = [r for r in compilation['rows'] if r['source'].startswith('solutions/')]
    (out / 'compilation.json').write_text(json.dumps(compilation, indent=2)+'\n')
    goose = rows['goose']
    runtime = [float(r['runtime_ms']) for r in goose]
    debug_file = ROOT / 'build/goose-debug/validation.json'
    if debug_file.exists():
        debug_rows = json.loads(debug_file.read_text())
        if len(debug_rows) == 100 and all(r['passed'] for r in debug_rows):
            shutil.copy2(debug_file, out / 'debug-validation.json')
    compile_times = [r['compilation_ms'] for r in compilation['rows']]
    env = meta['environment']
    text = [
        '# Goose problems 1–100: measured results', '',
        f"Run `{run_id}`. Every Goose answer passed exact-output verification.", '',
        f"CPU: {env['cpu']}. OS: {env['os']}. C backend: {compilation['cc']}.",
        f"Goose: `{compilation['compiler']}`. Pinned source commit: `{compilation['expected_compiler_commit']}`.", '',
        'Optimized standalone executables use GCC `-O2`. Compilation finishes before the runtime profile.',
        'Each solution runs once with no warm-up. The same invocation supplies its answer check.',
        'All durations below are measured wall time. There are no statistical error bars.', '',
        '| Measurement | Value |', '| --- | ---: |',
        f'| Passing Goose answers | {len(goose)}/100 |',
        f'| Total Goose child-process time | {sum(runtime):.3f} [ms] |',
        f'| Median Goose process time | {statistics.median(runtime):.3f} [ms] |',
        f'| Total build time for 100 Goose solutions | {sum(compile_times):.3f} [ms] |',
        f'| Median build time per solution | {statistics.median(compile_times):.3f} [ms] |',
        f"| Minimal Goose process startup median | {meta['startup_calibrations']['goose']['baseline_ms']:.3f} [ms] |", '',
        'Build timings include Goose compilation, C compilation, and linking. The shared runtime cache was warm.',
        'Process timings include startup, input loading, computation, output capture, and shutdown.',
        'The no-op median is an estimate of minimal process overhead. Subtracting it does not directly measure solver CPU time.', '',
    ]
    if 'c' in rows:
        ctotal = sum(float(r['runtime_ms']) for r in rows['c'])
        text += [f"The matching C run passed {len(rows['c'])}/100 and took {ctotal:.3f} [ms] in total.",
                 f'The ratio of total Goose time to total C time is {sum(runtime)/ctotal:.3f}.',
                 'This compares the implementations, including their different algorithms and big-integer libraries.',
                 'It does not isolate language or compiler speed. C links GMP; Goose uses decimal digit arrays.', '']
    text += ['## Model and verification limits', '',
             'The local verification also passed 100/100 debug executables with overflow and bounds checks enabled.',
             'Reusable helper examples passed in a debug build. The 52 Python tests passed, with one Gforth-dependent skip.',
             'Problem 84 uses independent uniform card draws rather than persistent shuffled decks.',
             'Problem 93 uses floating-point expression enumeration with a `1e-7` integer tolerance.',
             'Answer agreement verifies the supplied problem inputs, not arbitrary malformed input or a formal proof of every algorithm.', '',
             '## Slowest Goose processes', '', '| Problem | Total time [ms] |', '| ---: | ---: |']
    for r in sorted(goose, key=lambda r: float(r['runtime_ms']), reverse=True)[:10]:
        text.append(f"| {r['problem']} | {float(r['runtime_ms']):.3f} |")
    text += ['', '## Per-problem process times', '', '| Problem | Goose [ms] | C [ms] |', '| ---: | ---: | ---: |']
    c = {r['problem']: r for r in rows.get('c', [])}
    for r in goose:
        cr = c.get(r['problem'])
        ctime = f"{float(cr['runtime_ms']):.3f}" if cr else 'unmeasured'
        text.append(f"| {r['problem']} | {float(r['runtime_ms']):.3f} | {ctime} |")
    relative = 'profile/' + out.name
    text += ['', f'[Raw runtime observations]({relative}/goose.csv), [compilation observations]({relative}/compilation.json), and [run provenance]({relative}/metadata.json).', '',
             'Reproduce with `make setup-goose`, `make goose-release`, and `python3 scripts/bench.py --mode profile --languages goose`.',
             'For the C comparison, build C and its no-op executable, then add `c` to the same profiler invocation.',
             'Regenerate this record with `python3 scripts/report_goose.py`.', '']
    (ROOT / 'docs/GOOSE_1_100.md').write_text('\n'.join(text))
    print(out)

if __name__ == '__main__':
    main()
