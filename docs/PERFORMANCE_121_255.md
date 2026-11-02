# Rust and Factor performance, problems 121–255

This report records the validated profile run
`20261102T034707.905359Z-3b7bd12b`. Rust covers all 135 problems in the range;
Factor covers the 71 implemented problems listed in the manifest. Every program
was executed exactly once with a 60-second timeout. Compilation was completed
before timing, while process startup, vocabulary loading, computation, output,
and teardown are included in wall time.

The machine was Linux x86-64 on an AMD Ryzen AI MAX+ 395 with 32 logical CPUs.
Rust used `rustc 1.99.1` release builds. Factor used the repository's bundled
Factor VM. The raw CSV, metadata, and SVG/PNG/PDF plots are under
`build/profile/20261102T034707.905359Z-3b7bd12b/`; `build/profile/latest-plots/`
points at the same published charts.

## Results

| Measurement | Rust | Factor |
| --- | ---: | ---: |
| Declared programs in 121–255 | 135 | 71 |
| Passed | 135 | 71 |
| Timeouts/errors/wrong answers | 0 | 0 |
| Total wall time for declared coverage | 60.999996 s | 139.564389 s |
| Median program wall time | 7.842 ms | 1,734.784 ms |
| No-op startup median (5 samples) | 0.452 ms | 75.895 ms |
| Estimated execution residual total | 60.939491 s | 134.175874 s |

Totals over different coverage sets are not directly comparable. On the matched
71-problem subset, Rust totaled 5.655023 seconds and Factor totaled 139.564389
seconds. Their median wall times were 0.900 ms and 1,734.784 ms respectively.
Factor did not win an individual matched observation in this run.

That gap is not an intrinsic language benchmark. Most new Factor entrypoints load
and compile the large shared `euler.extended` vocabulary in a fresh VM process.
The no-op baseline measures only minimal VM startup, so vocabulary parsing and
compilation remain in the execution residual. The implementations also differ in
data representation and algorithm details. The numbers answer “how long do these
repository programs take under the harness?”, not “how fast is each language?”

## Slowest observations

| Rust problem | Wall time | Factor problem | Wall time |
| ---: | ---: | ---: | ---: |
| 185 | 19.907 s | 251 | 17.932 s |
| 154 | 5.038 s | 189 | 7.527 s |
| 223 | 4.388 s | 255 | 5.085 s |
| 251 | 4.336 s | 139 | 2.897 s |
| 142 | 3.510 s | 149 | 2.316 s |
| 229 | 3.197 s | 186 | 2.231 s |
| 153 | 2.927 s | 159 | 2.175 s |
| 212 | 2.641 s | 204 | 2.037 s |

Rust problem 185 is a genuine constraint search and remains comfortably below
the 60-second per-program limit. Factor problem 251 is the largest native
enumeration in the completed subset. No result was substituted with a reference
answer constant to improve timing.

## Reproduction

```bash
scripts/run_tests.sh --extended-range
make build/bench/noop-rust
python3 scripts/bench.py --mode profile --timeout 60 --startup-samples 5 \
  --languages rust factor --problem-range 121 255
MPLCONFIGDIR=/tmp/project-euler-matplotlib make plots
```

The runner deliberately performs no warm-up and reports one observation per
program. It therefore makes no statistical uncertainty claim.
