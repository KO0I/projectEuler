# Goose problems 1–100: measured results

Run `20261102T074523.340714Z-f8e9a92c`. Every Goose answer passed exact-output verification.

CPU: AMD EPYC 9V74 80-Core Processor. OS: Linux-6.18.57-x86_64-with-glibc2.39. C backend: gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0.
Goose: `goose v2026.3-265-geadca409 (runtime 3025e5810379)`. Pinned source commit: `eadca409f68210893f366e60d0c2f0dc29a339be`.

Optimized standalone executables use GCC `-O2`. Compilation finishes before the runtime profile.
Each solution runs once with no warm-up. The same invocation supplies its answer check.
All durations below are measured wall time. There are no statistical error bars.

| Measurement | Value |
| --- | ---: |
| Passing Goose answers | 100/100 |
| Total Goose child-process time | 10247.839 [ms] |
| Median Goose process time | 3.125 [ms] |
| Total build time for 100 Goose solutions | 12169.479 [ms] |
| Median build time per solution | 113.722 [ms] |
| Minimal Goose process startup median | 1.148 [ms] |

Build timings include Goose compilation, C compilation, and linking. The shared runtime cache was warm.
Process timings include startup, input loading, computation, output capture, and shutdown.
The no-op median is an estimate of minimal process overhead. Subtracting it does not directly measure solver CPU time.

The matching C run passed 100/100 and took 4010.844 [ms] in total.
The ratio of total Goose time to total C time is 2.555.
This compares the implementations, including their different algorithms and big-integer libraries.
It does not isolate language or compiler speed. C links GMP; Goose uses decimal digit arrays.

## Model and verification limits

The local verification also passed 100/100 debug executables with overflow and bounds checks enabled.
Reusable helper examples passed in a debug build. The 52 Python tests passed, with one Gforth-dependent skip.
Problem 84 uses independent uniform card draws rather than persistent shuffled decks.
Problem 93 uses floating-point expression enumeration with a `1e-7` integer tolerance.
Answer agreement verifies the supplied problem inputs, not arbitrary malformed input or a formal proof of every algorithm.

## Slowest Goose processes

| Problem | Total time [ms] |
| ---: | ---: |
| 60 | 7894.006 |
| 70 | 447.312 |
| 73 | 417.727 |
| 59 | 195.113 |
| 87 | 99.142 |
| 80 | 97.635 |
| 58 | 95.149 |
| 92 | 91.428 |
| 74 | 77.683 |
| 95 | 69.754 |

## Per-problem process times

| Problem | Goose [ms] | C [ms] |
| ---: | ---: | ---: |
| 1 | 0.850 | 0.891 |
| 2 | 0.900 | 0.708 |
| 3 | 0.868 | 0.707 |
| 4 | 2.065 | 1.704 |
| 5 | 0.978 | 0.777 |
| 6 | 1.062 | 0.765 |
| 7 | 4.650 | 4.968 |
| 8 | 1.314 | 1.186 |
| 9 | 1.010 | 1.559 |
| 10 | 10.063 | 6.829 |
| 11 | 1.272 | 1.152 |
| 12 | 4.737 | 4.917 |
| 13 | 1.926 | 1.606 |
| 14 | 30.756 | 185.847 |
| 15 | 1.345 | 1.198 |
| 16 | 1.627 | 1.234 |
| 17 | 1.212 | 1.002 |
| 18 | 0.949 | 0.734 |
| 19 | 0.988 | 1.064 |
| 20 | 1.274 | 0.977 |
| 21 | 7.890 | 5.145 |
| 22 | 3.397 | 2.348 |
| 23 | 32.513 | 7.146 |
| 24 | 7.106 | 1.117 |
| 25 | 9.082 | 0.896 |
| 26 | 1.594 | 1.387 |
| 27 | 20.102 | 22.581 |
| 28 | 1.358 | 1.223 |
| 29 | 17.931 | 6.277 |
| 30 | 4.779 | 3.061 |
| 31 | 1.239 | 1.559 |
| 32 | 1.419 | 1.199 |
| 33 | 1.002 | 1.049 |
| 34 | 18.857 | 17.050 |
| 35 | 6.009 | 3.981 |
| 36 | 7.196 | 3.632 |
| 37 | 61.476 | 1.136 |
| 38 | 2.272 | 1.723 |
| 39 | 5.109 | 36.750 |
| 40 | 1.648 | 10.924 |
| 41 | 2.784 | 1.294 |
| 42 | 1.683 | 0.981 |
| 43 | 22.292 | 1.477 |
| 44 | 8.394 | 47.988 |
| 45 | 2.471 | 1.372 |
| 46 | 1.551 | 5.937 |
| 47 | 3.203 | 46.301 |
| 48 | 3.028 | 1.582 |
| 49 | 11.145 | 4.767 |
| 50 | 7.202 | 4.872 |
| 51 | 26.850 | 7.111 |
| 52 | 3.047 | 1.756 |
| 53 | 1.207 | 1.042 |
| 54 | 1.903 | 1.194 |
| 55 | 32.328 | 7.314 |
| 56 | 4.802 | 4.720 |
| 57 | 3.044 | 2.577 |
| 58 | 95.149 | 10.603 |
| 59 | 195.113 | 23.146 |
| 60 | 7894.006 | 1037.518 |
| 61 | 1.658 | 1.572 |
| 62 | 1.825 | 1.877 |
| 63 | 1.170 | 0.863 |
| 64 | 4.460 | 3.491 |
| 65 | 1.656 | 1.079 |
| 66 | 7.577 | 2.020 |
| 67 | 1.046 | 1.256 |
| 68 | 37.932 | 1.902 |
| 69 | 1.395 | 0.898 |
| 70 | 447.312 | 323.955 |
| 71 | 3.320 | 15.744 |
| 72 | 21.714 | 10.410 |
| 73 | 417.727 | 12.449 |
| 74 | 77.683 | 13.587 |
| 75 | 25.639 | 12.918 |
| 76 | 1.350 | 1.239 |
| 77 | 1.482 | 1.149 |
| 78 | 60.238 | 18.597 |
| 79 | 1.366 | 1.586 |
| 80 | 97.635 | 4.948 |
| 81 | 2.677 | 2.008 |
| 82 | 1.587 | 1.433 |
| 83 | 48.775 | 66.394 |
| 84 | 6.070 | 1314.239 |
| 85 | 1.351 | 1.454 |
| 86 | 15.636 | 39.958 |
| 87 | 99.142 | 9.365 |
| 88 | 2.317 | 2.473 |
| 89 | 1.539 | 1.715 |
| 90 | 1.779 | 1.344 |
| 91 | 8.844 | 1.004 |
| 92 | 91.428 | 402.659 |
| 93 | 14.121 | 15.864 |
| 94 | 1.733 | 1.492 |
| 95 | 69.754 | 41.087 |
| 96 | 6.567 | 6.223 |
| 97 | 26.246 | 1.561 |
| 98 | 27.642 | 5.335 |
| 99 | 1.302 | 98.379 |
| 100 | 2.116 | 1.758 |

[Raw runtime observations](profile/20261102T074523.340714Z-f8e9a92c-goose/goose.csv), [compilation observations](profile/20261102T074523.340714Z-f8e9a92c-goose/compilation.json), and [run provenance](profile/20261102T074523.340714Z-f8e9a92c-goose/metadata.json).

Reproduce with `make setup-goose`, `make goose-release`, and `python3 scripts/bench.py --mode profile --languages goose`.
For the C comparison, build C and its no-op executable, then add `c` to the same profiler invocation.
Regenerate this record with `python3 scripts/report_goose.py`.
