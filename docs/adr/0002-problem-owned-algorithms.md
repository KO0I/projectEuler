# ADR-0002: Each problem owns its algorithm

Status: Accepted. Supersedes the centralized solver organization used after ADR-0001.

## Ownership

Each C source under `solutions/c/`, Rust binary under `solutions/rust/src/bin/`,
and numbered Gforth file under `solutions/forth/` contains its problem algorithm.
Problem-specific search routines, constants, buffers, and tests belong with that
algorithm. Running a problem does not require selecting its number in a library.

For example, C problem 7 counts primes in `10_001st_prime_7.c` and calls the
shared `prime(n)` predicate. Rust problem 7 owns the same loop in `bin/p7.rs`.
Gforth `p7.fs` defines and invokes `p7`, using `is-prime?` from `math.fs`.

Reusable libraries expose operations on their inputs: primality, divisors,
integer square roots, big integers, modular arithmetic, input parsing, and
parameterized combinatorial routines. They contain no complete fixed-input
problem answers, number-to-solver dispatch tables, or imports of problem files.
The reciprocal-denominator search shared by problems 108 and 110 takes an
explicit solution-count threshold; each problem supplies its own threshold.

## Files and dependencies

| Language | Algorithm owner | Reusable dependencies |
| --- | --- | --- |
| C | `solutions/c/<title>_<N>.c` | `support/c/euler_common.h`, `euler_common.c`, `euler_extra.h` |
| Rust | `solutions/rust/src/bin/p<N>.rs` | `project_euler::extra_helpers`, bigint crates |
| Gforth | `solutions/forth/p<N>.fs` | `math.fs`, `big.fs` |

C and Rust use a local `solve()` function where separating computation from
stdout is useful. It takes no problem ID and stays inside the problem file.
Forth tests define `euler-testing` to load selected problem definitions without
executing their final print-and-exit statements.

## Migration and alternatives

`scripts/migrate_problem_ownership.py` records the mechanical extraction of the
pre-refactor source tree. It copies algorithm bodies and follows their helper
dependencies. It refuses to rerun after the old dispatcher files are removed.
The extracted files are now maintained directly; the script is migration history,
not a source generator used by ordinary builds.

Keeping central dispatchers behind small wrappers fails the ownership requirement.
Moving those dispatchers into smaller groups still makes a library own complete
problem answers. Copying all mathematical helpers into every file would obscure
the reusable mathematics and duplicate fixes. The chosen layout keeps the
problem algorithm local while preserving shared operations on explicit inputs.

## Verification

`scripts/check_language_coverage.py` checks local solver ownership and rejects the
old C, Rust, and Gforth dispatch paths. Native helper tests continue to exercise
mathematical operations and selected problem routines. `make unit-rust` includes
binary-local tests, so moving a test alongside its algorithm preserves coverage.
The answer runner still launches each problem separately with a 60-second limit.

The [refactor validation record](../VALIDATION_PROBLEM_OWNERSHIP.md) retains the
new answer checks and distinguishes them from earlier measurements.

Factor is outside this migration. Some Factor entry points already contain their
algorithms; others use its existing problem vocabulary modules.
