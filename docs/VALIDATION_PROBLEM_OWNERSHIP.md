# Validation after moving algorithms into problem files

The [ownership refactor](adr/0002-problem-owned-algorithms.md) moves the 37
centralized C algorithms and all 120 Rust and Gforth algorithms into their
individual problem files. The other 83 C algorithms already had that ownership.
The answer manifest and command-line output contract are preserved.

## Answer checks

Each solver was invoked once with a 60-second limit using `scripts/bench.py`.
Tests use the native compiler/interpreter and the real repository input files.

| Language | Result | Evidence |
| --- | --- | --- |
| C | 120/120 exact answers | [CSV](validation/problem_ownership/c.csv), [metadata](validation/problem_ownership/c-metadata.json) |
| Rust | 120/120 exact answers | [CSV](validation/problem_ownership/rust.csv), [metadata](validation/problem_ownership/rust-metadata.json) |
| Gforth | 115/120 exact answers; five timeouts | [CSV](validation/problem_ownership/forth.csv), [metadata](validation/problem_ownership/forth-metadata.json) |

The C run is `20261102T005700.767244Z-513d4232`; the Rust run is
`20261102T005906.577042Z-92b57fb8`. The runs used Linux x86-64 and the toolchain
versions recorded in their metadata. Other build and validation work could run
concurrently, so these durations are validation evidence, not a speed comparison.
The source export has no local Git metadata, so the runner records the revision
as unavailable.

Published CSV and JSON evidence uses answer suffixes such as `…9448`. The
unredacted values remain in `tests/answers.json`, which the test runner uses.

The Gforth run is `20261102T005541.284606Z-d9722a86`. It reproduced the existing
timeouts on problems 33, 49, 51, 88, and 98, with zero wrong answers or runtime
errors. The refactor does not resolve those remaining algorithm/runtime findings.

## Build and ownership checks

All C programs compiled with `-O2 -std=c11 -Wall -Wextra -Wpedantic -Werror`.
C helper tests and the independent example suites passed. All Rust binaries
compiled in release mode. Gforth helper and bigint tests passed. All 38 Python
tests passed, including loading each of the 120 Gforth files independently and
verifying that an unrelated solver is absent from the dictionary. All 15 official
input checksums matched.

Rust library and binary-local tests passed. Tests for number words, poker,
rational arithmetic, and logarithm intervals now live beside their problem
algorithms. Shared-library tests still exercise primality, divisors,
combinatorics, and reciprocal-denominator minimization. `make unit-rust` runs
both library and binary-local tests.

The Rust build exposed pre-existing invalid value-bearing `break` statements in
the `for` loops for problems 58, 86, and 115, plus a `u128`/`u64` division mismatch
in problem 106. These were corrected before answer validation. The moved Gforth
problem 99 includes its own floating-point best-score variable.

## Reproduction

```sh
python3 scripts/check_language_coverage.py
make check-c-coverage check-inputs
make -j4 c-release unit-c CFLAGS='-O2 -std=c11 -Wall -Wextra -Wpedantic -Werror'
make rust-release unit-rust unit-forth unit-python
python3 scripts/bench.py --mode test --timeout 60 --languages c rust forth
make docs
```

The rebuilt report points to the individual problem files. Its embedded profile
figures are explicitly labeled as preceding this refactor; no new performance
claim is inferred from moving the code.
