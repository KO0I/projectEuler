# ADR-0003: C through problem 255 and separate solution scaffolds

> Amended by [ADR 0004](0004-rust-255-factor-bounded-extension.md): Rust now
> covers 1–255 and Factor has 71 implemented entries in the later range.

Status: Accepted, 2026-11-01.

## Context

The repository has validated implementations in four languages through problem
120. The next implementation target is contiguous C coverage through problem 255,
while later work benefits from discoverable starter files in every supported
language. An incomplete file must not be confused with implementation coverage.

## Decision

C problems 121 through 255 become problem-owned algorithms under `solutions/c/`
and satisfy the existing Validated Solution contract. Each prints one canonical
one-line answer string, matches the versioned exact-string reference answer, and targets a 60-second execution
limit. High-risk algorithms receive small-bound comparisons or mathematical
invariant tests where practical.

Rust, Gforth, and Factor receive Solution Scaffolds for problems 121 through 255.
All four languages receive scaffolds from problem 256 through the catalog frontier.
The initial catalog is an offline snapshot of the official Project Euler archive
through problem 1017 on 2026-11-01.

Scaffolds live under `scaffolds/`, outside every solution directory. They return
failure if invoked and are excluded from manifests, ordinary builds, answer tests,
profiling, and implementation counts. `scripts/scaffold_euler.py` deterministically
materializes them from `docs/project_euler_catalog.json`; `--refresh` is the only
networked operation. CI checks the versioned snapshot and generated files without
querying the live site or treating 1017 as a permanent maximum.

## Consequences

The repository gains many small generated files, but their provenance and status
are mechanically auditable. A scaffold must be removed when its corresponding
solution is added. Catalog growth is an explicit maintainer action and cannot make
an otherwise unchanged build fail because Project Euler published a new problem.

## Rejected alternatives

Putting placeholders in `solutions/` would overstate coverage and feed incomplete
programs to wildcard builds. Querying Project Euler in CI would make validation
network-dependent and turn normal publication into an unrelated build failure.
Keeping only a generator without materialized files would not provide the requested
language-specific starter files.
