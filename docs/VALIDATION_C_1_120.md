# Validation of C problems 1 through 120

The extension adds 83 C solutions to the 37 already covered in the report.
All 120 programs produce their exact reference answers. The original C source
files, `support/c/euler_common.c`, and the existing LaTeX report are unchanged
from baseline commit `e44f23ed50de1df537097dfa62e339a688e128ba`.

## Executed checks

These results were obtained on Linux x86-64 with GCC 13.3.0 and GMP 6.3.0.

| Check | Result |
| --- | --- |
| Complete numbered C coverage | 120 of 120; 37 retained and 83 added |
| Strict C11 build | All 120 compiled with `-Wall -Wextra -Wpedantic -Werror` |
| Exact-answer execution | 120 of 120 passed |
| AddressSanitizer, leak detection, and UBSan | 120 of 120 passed, no diagnostics |
| Official input integrity | All 15 additional files match their pinned SHA-256 hashes |
| Existing and new C example suites | All passed |
| Python runner, coverage, and visualization tests | 19 passed |
| Portable modular multiplication | Passed the same GMP-oracle checks with native 128-bit support disabled |
| Independent algorithm audit | All targeted comparisons passed |

The first combined execution encountered two locally built ELF files without
their executable permission bits. Correcting those generated file modes restored
the expected answers without changing either algorithm. The first sanitizer run
also failed because the restricted execution environment prevented LeakSanitizer
from inspecting `/proc`. The complete suite was then executed in the supported
environment with `detect_leaks=1`; all 120 answers and sanitizer checks passed.
Neither failed infrastructure run is counted as a successful validation.

The final release execution recorded approximately 4,132 [ms] in child processes
and 4,140 [ms] for the sequential suite including runner overhead. This is one
observation on the available machine, not a benchmark distribution or a language
ranking. Compilation is excluded and process startup is included.

## Independent checks

The C tests exercise official examples and separate small-case calculations.
They cover poker tie-breaks and ace-low straights, reciprocal periods, path sums,
Pell equations, fraction counts, digit-factorial chains, integer triangles,
partitions, cuboid routes, product-sum numbers, Roman numerals, tile counts,
special-sum sets, and modular remainder identities.

The clique test compares the production minimum-clique search with exhaustive
enumeration on 128 small synthetic graphs. The Monopoly test compares all
1,105,920 combinations of state and six-sided dice with an independently written
rule interpreter. It retains the documented deck-order qualification; see
[the method catalog](C_1_120.md#two-model-qualifications).

`scripts/audit_c.py` supplies additional independent comparisons:

- Every one of 210 four-digit sets in problem 93 matches Python `Fraction` subset dynamic programming.
- Eight anagram pairs in problem 98 match enumeration of injective letter-to-digit maps, including all repeated-letter pairs in the input.
- Every per-digit maximum repetition count, prime count, and prime sum for two-, three-, and four-digit cases in problem 111 matches exhaustive trial division.
- Every one of the 63 nonempty digit subsets of 1 through 6 in problem 118 matches permutation-and-cut enumeration with unordered sets deduplicated.
- The special-sum seed and reciprocal divisor-count witnesses satisfy independent checks. Source review verifies that the finite search bounds include all better candidates.

## Reproduction

From the repository root:

```sh
make -j4 c-release unit-c CFLAGS='-O2 -std=c11 -Wall -Wextra -Wpedantic -Werror'
make test-c CFLAGS='-O2 -std=c11 -Wall -Wextra -Wpedantic -Werror'
make unit-python
make sanitize-c
make audit-c
```

`make unit-python` requires NumPy and Matplotlib for the existing figure tests.
`make sanitize-c` requires a Linux environment that permits LeakSanitizer process
inspection. It leaves leak detection enabled and fails if the environment cannot
support it.

The machine-readable [verification record](validation/c_1_120/verification.json)
lists source hashes and results. The same directory contains exact-answer CSV,
sanitizer output, and the independent audit results. The new pull-request workflow
repeats the strict build, example tests, input integrity, all 120 answers, and
sanitizer checks on a GitHub Linux runner. Its status is separate from this local
execution record.

Windows execution was not tested. The existing PowerShell `test-c` command builds
the same C files with GCC and GMP and uses the same 120-answer manifest.
