# Factor (not Gforth) solutions

This directory contains native Factor solutions for all 120 problems in the
repository manifest. Each `pN.factor` entry point loads reusable words from the
`euler.shared`, `euler.extra`, or `euler.advanced` vocabulary.

Use the [Factor programming language](https://factorcode.org/), not GNU coreutils
`factor` and not Gforth. Scripts use native bignums, sequences, ranges and lexical
locals. Run from the repository root with an installed Factor VM distribution:

```sh
FACTOR_VM=/absolute/path/to/Factor/factor make test-factor
FACTOR_VM=/absolute/path/to/Factor/factor make profile-factor
```

The benchmark runner supplies `solutions/factor` as a vocabulary root and uses
an empty generated home under `build/factor-home`. This matters with Factor
0.102: `-no-user-init` also disables `-roots`, so it cannot launch entry points
that use the shared Euler vocabularies. For a direct invocation, use:

```sh
mkdir -p build/factor-home
HOME="$PWD/build/factor-home" /absolute/path/to/Factor/factor \
  -roots="$PWD/solutions/factor" solutions/factor/p9.factor
```

Official release tarballs contain both the VM and its accompanying `factor.image`;
keep these together. CI pins the Linux x86-64 Factor 0.102 release archive.
There is deliberately no fallback to `/usr/bin/factor` (the unrelated integer
factorization utility on many Linux distributions).

All scripts compute their answer and print one decimal integer per invocation.
They use native bignums, sequences, ranges, lexical locals, and the versioned
input fixtures. `make test-factor` checks all 120 outputs against
`tests/answers.json`. A profiling run invokes each solution once; its separate
no-op processes estimate runtime startup without rerunning a solver.
