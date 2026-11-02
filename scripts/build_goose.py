#!/usr/bin/env python3
"""Build native Goose executables and retain separate compilation observations."""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time
from datetime import datetime, timezone

ROOT = Path(__file__).resolve().parents[1]
PIN = 'eadca409f68210893f366e60d0c2f0dc29a339be'
DEFAULT_COMPILER = ROOT / '.toolchains/goose/build/goose'

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', default=os.environ.get('GOOSE', str(DEFAULT_COMPILER)))
    parser.add_argument('--force', action='store_true', help='Measure a rebuild of every executable')
    parser.add_argument('--debug', action='store_true')
    args = parser.parse_args()
    compiler = str(Path(args.compiler).resolve()) if '/' in args.compiler else args.compiler
    output = ROOT / 'build' / ('goose-debug' if args.debug else 'goose')
    output.mkdir(parents=True, exist_ok=True)
    helper = ROOT / 'support/goose/euler.goose'
    sources = sorted((ROOT / 'solutions/goose').glob('*.goose'))
    sources.append(ROOT / 'support/bench/noop.goose')
    version = subprocess.check_output([compiler, 'version'], text=True).strip()
    cc = os.environ.get('GOOSE_CC', 'gcc')
    cc_version = subprocess.check_output([cc, '--version'], text=True).splitlines()[0]
    rows = []
    for source in sources:
        target = (ROOT / 'build/bench/noop-goose' if source.name == 'noop.goose' and not args.debug else output / source.stem)
        if os.name == 'nt':
            target = target.with_suffix('.exe')
        target.parent.mkdir(parents=True, exist_ok=True)
        digest = hashlib.sha256(source.read_bytes() + helper.read_bytes() +
                                f'{version}\n{cc_version}\n{args.debug}'.encode()).hexdigest()
        signature = target.with_suffix('.sha256')
        if not args.force and os.access(target, os.X_OK) and signature.exists() and signature.read_text() == digest:
            continue
        temporary = target.with_name(target.name + '.pending')
        temporary.unlink(missing_ok=True)
        cmd = [compiler, 'build', str(source), '--cc', cc, '-o', str(temporary)]
        if args.debug:
            cmd.append('--debug')
        start = time.perf_counter_ns()
        result = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
        elapsed = (time.perf_counter_ns() - start) / 1e6
        rows.append(dict(source=str(source.relative_to(ROOT)), compilation_ms=elapsed,
                         command=cmd, exit_code=result.returncode, build_inputs_sha256=digest,
                         stderr=result.stderr))
        print(f'{source.stem}: {elapsed:.3f} [ms]; exit {result.returncode}', flush=True)
        if result.returncode:
            print(result.stderr, flush=True)
        else:
            os.replace(temporary, target)
            signature.write_text(digest)
    report = dict(timestamp_utc=datetime.now(timezone.utc).isoformat(), compiler=version, expected_compiler_commit=PIN, cc=cc_version,
                  debug=args.debug, measurement='One build invocation per changed source; includes Goose and C compilation/linking. Runtime object cache may be warm.',
                  rows=rows)
    if rows:
        (output / 'compilation.json').write_text(json.dumps(report, indent=2) + '\n')
    return int(any(r['exit_code'] for r in rows))

if __name__ == '__main__':
    raise SystemExit(main())
