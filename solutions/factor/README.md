# Factor (not Gforth) solutions

This directory contains **12 native Factor** solutions, selected as a first independently
reviewable subset of the repository's 37 problems: 1, 2, 5, 6, 9, 10, 11, 16, 20, 25, 28, 39.

Use the [Factor programming language](https://factorcode.org/), not GNU coreutils
`factor` and not Gforth. Scripts use native bignums, sequences, ranges and lexical
locals. Run from the repository root with an installed Factor VM distribution:

```sh
FACTOR_VM=/absolute/path/to/Factor/factor make test-factor
FACTOR_VM=/absolute/path/to/Factor/factor make profile-factor
/absolute/path/to/Factor/factor -no-user-init solutions/factor/p9.factor
```

Official release tarballs contain both the VM and its accompanying `factor.image`;
keep these together. CI pins the Linux x86-64 Factor 0.101 release archive.
There is deliberately no fallback to `/usr/bin/factor` (the unrelated integer
factorization utility on many Linux distributions).

All scripts compute their answer; they do not print a reference constant. Problem
11 reads the same grid fixture as the C implementation. Native Factor bignums are
used by problems 16, 20 and 25 without artificial fixed-width overflow.
Scripts print one decimal integer per invocation, for the existing benchmark harness.

**Scope:** the other 25 problems are **not implemented in Factor** here.
A Factor subtotal is not compared with full 37-problem suite totals. Factor
is invoked exactly once *per implemented problem* in each independent profiling run.
No baseline timing has been claimed without an actual Factor execution.
