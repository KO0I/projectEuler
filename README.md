# Project Euler solutions in C, Rust, Gforth, and Factor

C, Rust, Gforth, and Factor have source entry points for **all 120 problems from 1 through 120**. Each C, Rust, and Gforth file owns its problem's algorithm and uses shared mathematical helpers. C and Rust pass all 120 exact-answer checks after the ownership refactor; the Factor 0.102 suite also has a complete passing validation record. A recorded 60-second-per-solver Gforth run passes 115; problems 33, 49, 51, 88, and 98 time out, with no wrong answers or runtime errors. Original attempts remain under `variants/`. This branch does not modify `master` until reviewed and merged.

- [Problem index](solutions/README.md)
- [The 83 new C methods, answers, bounds, and model qualifications](docs/C_1_120.md)
- [C 1–120 validation record](docs/VALIDATION_C_1_120.md)
- [Gforth 1–120 validation record](docs/VALIDATION_FORTH_1_120.md)
- [Validation after moving algorithms into problem files](docs/VALIDATION_PROBLEM_OWNERSHIP.md)
- [Factor implementations](solutions/factor/README.md)
- [Mathematical figure index](docs/visualization_index.md)
- [LaTeX explanations and red correction notes](explanations/latex/README.md)
- [Validation baseline](docs/VALIDATION.md)
- [Original-to-new path map](docs/renames.tsv)
- [Implementation and timing decisions](docs/adr/0001-c-rust-forth.md)
- [Problem ownership and shared-library boundaries](docs/adr/0002-problem-owned-algorithms.md)

## Layout

| Path | Purpose |
| --- | --- |
| `solutions/c/` | 120 title-named C programs: 37 retained and 83 added |
| `support/c/euler_common.c` | Reusable mathematical helpers, including primality, divisors, and digit sums |
| `support/c/euler_extra.h` | Exact arithmetic, primality, allocation, and input helpers for the new C programs |
| `solutions/c/variants/historical/` | Preserved original main C programs |
| `solutions/rust/src/lib.rs` | Library entry point exposing reusable mathematical and input helpers |
| `solutions/rust/src/bin/pN.rs` | Each problem's Rust algorithm, private helpers, and executable entry point |
| `solutions/forth/math.fs` | Reusable mathematical and input words; no problem dispatcher |
| `solutions/forth/big.fs` | Dynamic base-10000 arbitrary-precision arithmetic |
| `solutions/forth/pN.fs` | Each problem's Gforth algorithm, private state, and direct invocation |
| `solutions/factor/pN.factor` | 120 Factor language entry points; not GNU coreutils or Gforth |
| `tests/answers.json` | 120 exact reference answers and explicit per-language coverage |
| `tests/data/manifest.json` | Official URLs and SHA-256 hashes for 15 additional input files |
| `scripts/` | Validation, single-run profiling and plotting |
| `explanations/latex/` | Historical explanations plus labelled red corrections |

Historical Shell, Mathematica and LaTeX work remains under `solutions/`. Generated prime and palindrome tables are no longer required by the main solutions. Real input datasets remain versioned: `lsum`, `names.txt`, and `tests/data/p008.txt` / `p011.txt`.

Every C, Rust, and Gforth problem owns its algorithm in its numbered source file. Libraries provide parameterized mathematical operations and input handling. There is no `euler_solve(N)`, `project_euler::solve(N)`, or `euler-solve` dispatcher. Run `python3 scripts/check_language_coverage.py` to check these ownership rules. The original 37 C algorithms were moved out of the common module; the dated validation records describe the earlier layout.

## C only

For the C suite, install a C11 compiler, GMP development files, GNU Make, and Python 3. All problem inputs are included; solving does not require network access.

```sh
make -j4 c-release
make test-c                 # all 120 exact answers and input checksums
make unit-c                 # independent examples and arithmetic checks
make run-c PROBLEM=120      # build and run one problem by number
make sanitize-c            # Linux: all 120 under AddressSanitizer and UBSan
```

On Windows with GCC and GMP available through MSYS2/UCRT64, use `./scripts/test.ps1 -Action test-c`. Windows execution has not been verified in this change. `make fetch-inputs` can restore official datasets if needed; ordinary tests only verify the included files offline.

## Requirements

Use a 64-bit host with GCC or compatible C11 compiler, GMP development headers/libraries, stable Rust with Cargo, Gforth, the [Factor VM](https://factorcode.org/) (currently pinned to 0.102 in CI), and Python 3. Plotting additionally needs NumPy and Matplotlib; documentation needs `pdflatex`, Latin Modern and the standard packages listed in the LaTeX preamble. No network access is needed while solving the problems; initial dependency installation and Cargo fetching may require it. `Cargo.lock` records the dependency versions used by validation.

For example, on Debian/Ubuntu with Rust already installed:

```sh
sudo apt-get install build-essential libgmp-dev gforth python3-venv texlive-latex-recommended lmodern
python3 -m venv .venv
.venv/bin/python -m pip install numpy matplotlib
# Obtain Factor's own VM bundle and set FACTOR_VM to its executable:
export FACTOR_VM=/absolute/path/to/Factor/factor
make unit PYTHON=.venv/bin/python
make test PYTHON=.venv/bin/python
make profile PYTHON=.venv/bin/python
make plots-math PYTHON=.venv/bin/python
make docs
```

Run commands from the repository root. `make` builds all C executables, not only problem 2. `make test-c`, `make test-rust`, and `make test-forth` select one language. Native helper tests are available through `make unit-c`, `unit-rust`, `unit-forth`, and `unit-python`.

On Windows, use `./scripts/test.ps1 -Action test` or `-Action profile` with the same tools on PATH; MSYS2/UCRT64 can provide GCC and GMP. The script exists, but Windows execution has not yet been validated. These Forth solvers explicitly require 64-bit cells.

## Arch Linux: all four languages (including Factor)

Run from the repository root on **x86_64 Arch Linux**:

```sh
./setup.sh                    # pacman prerequisites + official Factor 0.102 VM
make run_profile_all          # build C/Rust; profile all 120 in each of 4 languages; plot
```

`setup.sh` installs `base-devel`, GMP, Gforth, Rust/Cargo, Python, NumPy,
Matplotlib, curl, and tar through `pacman`. It downloads the official **Factor
programming-language** distribution into `.toolchains/factor/`, preserving the
`factor.image` beside its VM. This is **not** GNU coreutils `factor`. Downloads
require network access, and `pacman` uses sudo unless invoked as root. Existing
Factor installations can be used with `FACTOR_VM=/absolute/path/to/factor`.
Run `./setup.sh --check` to inspect prerequisites without modifying your system;
`./setup.sh --no-pacman` only installs/checks Factor and existing packages.
The Factor official Linux binary is x86-64; for other architectures use a
separately built VM and `FACTOR_VM`. <https://factorcode.org/>

`make run_profile_all` is an alias for the full-suite `make profile` target.
It checks the input datasets and available toolchains, compiles optimized C
and Rust binaries, and executes every C, Rust, Gforth, and Factor solver **once**
with answer checking. Each language uses five separate minimal-startup probes.
CSV, metadata, and SVG/PNG/PDF charts are written to the **same**
`build/profile/<run-id>/` directory; `build/profile/latest.json` identifies
that run. The make command exits nonzero when an answer, timeout, or plotting
step fails. For slower machines, set `PROFILE_TIMEOUT=120`; adjust
`STARTUP_SAMPLES=5` independently. For example:

```sh
make run_profile_all PROFILE_TIMEOUT=120 STARTUP_SAMPLES=7
make plots                     # regenerate plots from the latest complete run
```

The Rust and Factor 0.102 entry points pass all 120 exact-answer checks in local
native validation. The recorded Gforth full-suite run passes 115 of 120 with a
60-second limit per solver; the other five are timeouts.
A failed benchmark remains a validation finding. The installer and full
four-language target have passed static checks but have not been executed on
Arch Linux here.

## Single-run profiling

```sh
make profile                         # all languages; builds first, then times once
make profile-c                       # C only
make profile-rust                    # Rust only
make profile-forth                   # Gforth only
make profile-factor                  # Factor 1–120 (requires Factor VM)
make test-factor                     # Validate Factor entry points (requires Factor VM)
make plots                           # existing measurements only; does not run solvers
make plots-math                      # deterministic mathematical visualizations
```

Each profile invokes each selected solution exactly once, with no warm-up. Validation checks the output from that same invocation. Total wall time uses `time.perf_counter_ns()` and includes process/interpreter startup, normal execution and output capture, but excludes compilation. In profile mode, five **separate no-op probes per language** estimate a minimal startup baseline; they do **not** re-run Euler solutions. The per-problem graphics stack a **gray, diagonally hatched startup estimate**, a **language-colored/patterned execution residual**, and a **dark line at the split boundary**. Complete-suite charts additionally separate runner overhead. The baseline is an approximation, not the true solver initialization time; module imports, fixture loading, I/O and shutdown remain in the execution residual. See [startup profiling methods and limitations](docs/STARTUP_PROFILING.md). Failures, timeouts and unavailable tools are explicit states, not zero-duration successes.

Every invocation creates its own `build/profile/<run-id>/` containing CSV measurements (including `startup_estimate_ms`, `execution_residual_ms` and split status), environment/revision/calibration JSON and SVG/PNG/PDF plots. `build/profile/latest.json` selects one complete run for `make plots`; old runs and languages are never pooled silently. The plot command also copies that run's figures to `build/profile/latest-plots/` for the documentation build. Test results use a separate `build/test-results/` directory. No confidence intervals or error bars are inferred from a single observation.

Per-language total bars measure the whole sequential language suite, stacked as startup baseline + execution residual + runner overhead; diamonds show summed child-process durations. A separate `matched_subset_totals` figure compares only the **intersection of implemented Euler IDs** in all selected languages, now 120 when all four are selected. Only a complete passing 120-problem run receives a full-suite total bar; missing or failed native executions remain explicit rather than being plotted as zero. Missing implementations are never plotted as zero. These measurements compare the current implementations and their algorithms, libraries, and input loading.

Mathematical figures go to `build/math_plots/` as SVG/PNG/PDF: the five original figures plus 18 additional figures covering the 27 topic names and official IDs listed in [the index](docs/visualization_index.md). Several are explicitly reduced examples rather than full Euler solutions.

## Documentation and CI

`make docs` builds `explanations/latex/build/project_euler_c_explanations.pdf` directly from the committed sources. Black text describes historical programs; red, explicitly labelled sections describe corrections. An appended cross-language chapter compares key C/Rust/Gforth/Factor snippets; a figure atlas embeds plots when generated first. Problems 13 and 17 now have C chapters too.

The new C validation workflow builds all 120 programs with warnings treated as errors, verifies official input hashes, runs independent examples and exact-answer checks, and checks every program with AddressSanitizer and UBSan. It preserves validation results for 30 days. The existing multi-language workflow also handles explicit per-language coverage and retains its figure/report generation. There are no scheduled builds. `make clean` removes generated build outputs.

The original C/Rust/Gforth validation baseline remains in `docs/VALIDATION.md`; the current extensions are recorded in `docs/VALIDATION_C_1_120.md` and `docs/VALIDATION_FORTH_1_120.md`. The existing LaTeX report retains its 37 chapters. The 83 new methods and source links are documented separately in `docs/C_1_120.md`.
