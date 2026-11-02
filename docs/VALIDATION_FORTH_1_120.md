# Validation of Gforth problems 1 through 120

This record describes the repair cycle before algorithms moved into individual
problem files. The [ownership-refactor validation](VALIDATION_PROBLEM_OWNERSHIP.md)
reproduces the same 115-pass, five-timeout result with the new layout.

The Gforth collection has 120 entry points. On the recorded 60-second-per-solver run, 115 produce the exact reference answer. Problems 33, 49, 51, 88, and 98 time out. The run has no wrong answers, runtime errors, or unavailable entries.

## Executed checks

The final test run used Gforth 0.7.3 on 64-bit Linux x86-64 with an AMD Ryzen AI MAX+ 395. Each solver was started once in a separate child process. Compilation and warm-up runs were not part of the measurement.

| Check | Result |
| --- | --- |
| Numbered Gforth entry points | 120 of 120 present and attempted |
| Exact-answer execution | 115 of 120 passed |
| Wrong answers | 0 |
| Runtime errors | 0 |
| Timeouts at 60 seconds | 5: problems 33, 49, 51, 88, and 98 |
| Forth helper and bigint tests | Passed |

The language suite took 360,277.741 ms of wall time. Child-process durations totaled 360,266.248 ms, of which 59,964.634 ms came from completed passing solvers. Timeout durations are attempted work, not successful timings. This is one observation and does not establish a performance distribution.

## Repairs covered by the run

The validation cycle first corrected wrong answers in problems 68, 80, 105, and 115. It then removed the runtime errors found in the full suite by fixing shared stack, bounds, and allocation defects. Problem 60 now allocates its adjacency matrix on the heap for the exact prime count and releases it after use, instead of reserving a fixed 16,000,000-byte dictionary block. The heap allocation still fails explicitly if the host cannot provide the requested memory.

The five remaining failures are bounded-runtime findings. They do not demonstrate an incorrect answer, because none of those processes completed before the one-minute limit.

## Reproduction

From the repository root:

```sh
make unit-forth
python3 scripts/bench.py --mode test --timeout 60 --languages forth
make profile-forth PROFILE_TIMEOUT=60 STARTUP_SAMPLES=5
```

The exact-answer run was `20261101T231511.022487Z-e874145e`. Its generated CSV and metadata are under `build/test-results/` while that build tree is retained. A profile run writes SVG, PNG, and PDF graphs below `build/profile/<run-id>/plots/`. The per-problem graph marks timeouts separately from wrong answers and runtime errors; incomplete suites do not receive a total-time bar.

The corresponding profile run was `20261101T232346.029681Z-e4ade9cd`. It reproduced 115 passes and the same five timeouts, with 360,156.482 ms of attempted child time and 360,169.809 ms of suite wall time. Its five independent no-op probes produced a 1.305 ms median Gforth startup estimate. The report build uses the graphs copied from this single run to `build/profile/latest-plots/`.

Passing these fixed Project Euler inputs is evidence for those inputs, not proof of a general-purpose implementation. Other Forth systems and 32-bit cells are unsupported and were not tested.
