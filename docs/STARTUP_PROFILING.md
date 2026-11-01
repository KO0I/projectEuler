# Startup and execution estimates in Project Euler performance plots

Each selected solution is **run once**, with no warm-up. The runner times one child
process using Python's `time.perf_counter_ns()` around `subprocess.run()`; this
includes subprocess spawning, runtime/interpreter initialization, module loading,
computation, output capture, and process teardown. Compilation is excluded.

## Calibrating a baseline without rerunning solutions

In *profile* mode only, the runner measures five independent minimal no-op
processes for each language, using the same executable style and runtime:

| Language | No-op source | Profile-build method |
| --- | --- | --- |
| C | `support/bench/noop.c` | C11 compiler using `CFLAGS` |
| Rust | `support/bench/noop.rs` | optimized `rustc`, LTO thin |
| Gforth | `support/bench/noop.fs` | same `gforth` interpreter |
| Factor | `support/bench/noop.factor` | Factor VM with the same vocabulary roots and an isolated generated home |

The **median** of the five no-op durations is a *minimal process/runtime startup
baseline*, not a direct observation of any solver's startup or of time-to-first
instruction. It includes parent-side process creation and capture overhead. A
minimal no-op does not load the solver's actual data, libraries, or vocabulary;
that extra initialization is included in the residual below. The build targets
prepare the two compiled no-op binaries before timing begins. Customize the
probe count with `--startup-samples N` (1–99) when invoking `scripts/bench.py`.

For each *passing* solver, the CSV and graphics report:

\[
T_{\mathrm{startup,estimated}}[\mathrm{ms}]
  = \min(T_{\mathrm{wall,observed}},\; T_{\mathrm{no-op,median}})
\]

\[
T_{\mathrm{execution,residual}}[\mathrm{ms}]
  = T_{\mathrm{wall,observed}} - T_{\mathrm{startup,estimated}}
\]

This decomposition is **additive but approximate**. `execution_residual_ms` is
not strictly algorithm-only time: it also includes the solver's imports,
initialization, fixture loading, output, and shutdown. When the no-op baseline
exceeds the actual solver wall time, startup is capped at the observed duration
and `split_status` is `capped`, not treated as a negative execution duration.
The uncapped no-op median and every individual sample remain in `metadata.json`.
These are independent samples of no-ops, not repeated solution timings; no
confidence interval for solver performance is inferred.

If a no-op probe cannot execute, or if an older profile has no startup fields,
its solution's total is left **undivided**: no startup or execution value is
fabricated. Failed, timed-out, or missing solutions do not appear as zero-time
successes.

## Chart encoding

The plots `runtime_by_problem`, `totals`, and `matched_subset_totals` use:

- **Light gray with dense diagonal hatch** for the startup baseline estimate.
- **Distinct per-language colors and hatching** for execution residuals, stacked
  *after* startup so their sum equals the actually observed child duration.
- **A dark horizontal boundary line** at the end of the startup segment.
- **Light dotted runner-overhead segment** on complete-suite wall-clock bars;
  the white diamond marks the sum of child process durations.

The same information is retained when printing grayscale. Per-problem and
matched-subset charts use logarithmic axes, so *visual bar heights* there are
not proportional to time; use the recorded CSV for quantitative comparison.
The suite-wall chart uses a linear axis, with an explicit runner-overhead
segment. Older schema-2 plots are still accepted but drawn without a split.

## Output schema and reproduction

`make profile` (or `make profile-c`, `profile-rust`, `profile-forth`,
`profile-factor`) writes an **individual schema-3 profile run** at
`build/profile/<run-id>/`. `metadata.json` records the calibration method,
per-language median, samples, commands, and environment. Language CSV files
retain `runtime_ms` and add `startup_estimate_ms`, `execution_residual_ms`,
and `split_status`. `make plots` reuses **exactly one run** and never creates
new measurements. SVG, PNG, and PDF figures are placed under that run's
`plots/` directory.

Do not read these figures as measured time spent exclusively *inside* an
algorithm, or as an intrinsic language speed comparison. For precise
per-program initialization boundaries, each language runtime would need an
explicit first-instruction timing marker; that is outside the current
nonintrusive profile protocol.
