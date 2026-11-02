# ADR-0004: Rust through 255 and a bounded Factor extension

Status: Accepted, 2026-11-02.

## Context

ADR-0003 established C coverage through problem 255 and failure scaffolds for
later language work. The next request was to add equivalent Rust and Factor
solutions through the same boundary, with permission to stop individual problems
when no clear path existed or execution became excessive, provided the attempt
was documented.

## Decision

Rust now has problem-owned algorithms for every problem 1–255. Factor has 191
declared implementations in total: the original 1–120 plus 71 selected problems
from 121–255. The other 64 Factor problems remain explicit scaffolds and are
documented in `docs/FACTOR_121_255_EXCEPTIONS.md`.

Coverage is explicit and may be sparse. `tests/answers.json` declares each
language/problem pair, the structural audit requires exact agreement with source
entrypoints, and the benchmark runs only declared pairs. The scaffold generator
checks canonical solution paths and omits a scaffold as soon as its implementation
exists.

## Consequences

Rust and Factor can be validated together with
`scripts/run_tests.sh --extended-range`, or one bounded language slice can be run
with `--language-range LANGUAGE FIRST LAST`. The profile report must compare the
71-problem intersection when discussing Rust versus Factor; full totals cover
different problem sets.

Factor's large shared extension vocabulary favors implementation speed and
reviewability over startup performance. A later refactor may split it into
smaller vocabularies without changing solver contracts.
