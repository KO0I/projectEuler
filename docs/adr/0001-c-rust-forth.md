# ADR-0001 — Parallel C, Rust, and Forth solutions

**Status:** Accepted, 2026-11-01.

## Context

The repository preserves historical C approaches, some incomplete or incorrect.
The next stage repairs them and adds independent Rust and Forth implementations.
It must not silently replace historical evidence or mistake a reference answer
for proof of a general algorithm.

## Decision

Cover the 37 existing problem numbers in every language. Keep Git history and
historical variants, and use distinct main files named after the problem title.
Use GNU C + GMP where required, stable Rust edition 2024 with one Cargo package
and \`num-bigint\` when appropriate, and Gforth with a documented base-10000
little-endian dynamic-limb arbitrary-precision module.

The shared Python harness checks exact numeric stdout against versioned reference
answers. C and Rust produce optimized native binaries; Forth runs under Gforth.
Linux Make recipes and a Windows PowerShell entry point use the same harness.

Profiling records one external wall-clock invocation per language/problem and
does not include compilation. Preserve failures as failures, never zero durations.
Matplotlib generates accessible hatch-coded runtime comparisons and mathematical
figures with explicit sample sizes. Data files stay versioned, and generated
primes/palindromes should no longer require historical external tables.

LaTeX chapters retain historical explanations; corrections are explicitly
labelled and colored red, with text labels for grayscale readability.

## Consequences

The Forth implementation requires explicit memory management and carry/borrow
tests. A single measurement provides comparative observations, not error
estimates or stable rankings. Cross-language output parity is necessary but
insufficient for full verification. Rust dependency resolution may require
network access at build time; pin dependencies in Cargo.lock.

## Rejected alternatives

A literal bug-for-bug translation would perpetuate incorrect results. A
repeated-run benchmarking protocol was rejected in favor of exactly one
execution per solution. Calling the C GMP API from Forth was rejected in favor
of a self-contained arithmetic library.
