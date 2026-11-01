# Validation baseline — 2026-11-01

The first complete same-machine run was [Actions run 41283644643](https://github.com/KO0I/projectEuler/actions/runs/41283644643), for branch commit `c47ec5b445c9601b29a8e7c9ce727662adea049c` (GitHub tested PR merge revision `e475c5338df3bf48ab13ac0a8ef0954f4f0e6f26`). This is a dated baseline, not a claim that later edits have already passed.

| Language | Expected answers passed | Sequential suite wall time [ms] | Sum of child durations [ms] |
| --- | ---: | ---: | ---: |
| C | 37/37 | 793.683003 | 791.216771 |
| Rust | 37/37 | 733.775981 | 731.456817 |
| Forth | 37/37 | 18273.287110 | 18269.022986 |

All 111 executables were invoked once in the profile, without warm-ups or repeated trials. No problem exceeded the 30-second limit. The slowest observation was Forth problem 44 at 5078.941717 [ms]. These results do not establish a stable ranking or a statistical uncertainty interval.

Environment recorded by the runner: Linux x86_64, AMD EPYC 7763, 4 exposed logical CPUs, GCC 13.3.0 with `-O2 -std=c11 -Wall -Wextra`, Rust 1.99.1 release builds and Gforth 0.7.3. C and Forth read problem fixtures at runtime; Rust embeds them. Differences in algorithms and arithmetic libraries also affect timing.

Additional successful steps: C helper tests, Python runner tests (including exact single invocation and nonzero failure reporting), Rust helper tests, Forth solver-helper tests, and bigint regressions covering carries, borrows, signs, multiplication, division, normalized zero and dynamic growth to 1201 decimal digits. Mathematical plots and the exact-source 40-page LaTeX PDF compiled successfully.

Reference-answer agreement is not a proof for arbitrary inputs. The solvers target these fixed Project Euler problems; for example, problem 44 currently searches pentagonal indices below 5000 and passes the supplied reference answer, rather than exposing a general minimization API. Windows execution and other Forth implementations have not been validated. Full Forth programs require 64-bit cells and use Gforth locals explicitly.

The artifact contains the raw observations and metadata. Compare later results only within a single recorded run; never combine partial totals from separate machines or runs.
