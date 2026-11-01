# Project Euler — C, Rust, Gforth, and Factor (partial)

The 37 problems already represented in this repository now have C, Rust and Gforth implementations. An additional 12 have **new, initially unvalidated** Factor-language implementations. Original attempts, including unfinished work, are preserved rather than silently overwritten. This branch is development work; it does not modify `master` until reviewed and merged.

- [Problem index](solutions/README.md)
- [Factor implementation subset](solutions/factor/README.md)
- [Mathematical figure index](docs/visualization_index.md)
- [LaTeX explanations and red correction notes](explanations/latex/README.md)
- [Validation baseline](docs/VALIDATION.md)
- [Original-to-new path map](docs/renames.tsv)
- [Implementation and timing decisions](docs/adr/0001-c-rust-forth.md)

## Layout

| Path | Purpose |
| --- | --- |
| `solutions/c/` | 37 title-named C entry points |
| `support/c/euler_common.c` | Corrected C algorithms and shared helpers |
| `solutions/c/variants/historical/` | Preserved original main C programs |
| `solutions/rust/src/lib.rs` | Independent Rust algorithms |
| `solutions/rust/src/bin/pN.rs` | One Rust executable per problem |
| `solutions/forth/solvers.fs` | Independent Gforth algorithms |
| `solutions/forth/big.fs` | Dynamic base-10000 arbitrary-precision arithmetic |
| `solutions/forth/pN.fs` | One Gforth entry point per problem |
| `solutions/factor/pN.factor` | 12 Factor language scripts; not GNU coreutils or Gforth |
| `tests/answers.json` | Common problem manifest and exact expected answers |
| `scripts/` | Validation, single-run profiling and plotting |
| `explanations/latex/` | Historical explanations plus labelled red corrections |

Historical Shell, Mathematica and LaTeX work remains under `solutions/`. Generated prime and palindrome tables are no longer required by the main solutions. Real input datasets remain versioned: `lsum`, `names.txt`, and `tests/data/p008.txt` / `p011.txt`.

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

## Single-run profiling

```sh
make profile                         # all languages; builds first, then times once
make profile-c                       # C only
make profile-rust                    # Rust only
make profile-forth                   # Gforth only
make profile-factor                  # Factor subset only (12 problems)
make test-factor                     # Validate Factor subset
make plots                           # existing measurements only; does not run solvers
make plots-math                      # deterministic mathematical visualizations
```

Each profile invokes each selected solution exactly once, with no warm-up. Validation checks the output from that same invocation. Timing uses `time.perf_counter_ns()` and includes process/interpreter startup, normal execution and output capture, but excludes compilation. Failures, timeouts and unavailable tools are explicit states, not zero-duration successes.

Every invocation creates its own `build/profile/<run-id>/` containing CSV measurements, environment/revision JSON and SVG/PNG/PDF plots. `build/profile/latest.json` selects one complete run for `make plots`; old runs and languages are never pooled silently. Test results use a separate `build/test-results/` directory. No confidence intervals or error bars are inferred from a single observation.

Per-language total bars measure the whole sequential language suite; diamonds show summed child-process durations. A separate `matched_subset_totals` figure compares the sum of per-problem process times for the **intersection of implemented Euler IDs** in all selected languages (12 when all four are selected), without suggesting this is a 37-problem suite. Only complete passing full 37-problem suites receive total bars. The incomplete Factor subset has a separate pass count, and missing implementations are never plotted as zero. These measurements compare the current implementations, not intrinsic language speed: algorithms, arithmetic libraries and input loading differ.

Mathematical figures go to `build/math_plots/` as SVG/PNG/PDF: the five original figures plus 18 additional figures covering the 27 topic names and official IDs listed in [the index](docs/visualization_index.md). Several are explicitly reduced examples rather than full Euler solutions.

## Documentation and CI

`make docs` builds `explanations/latex/build/project_euler_c_explanations.pdf` directly from the committed sources. Black text describes historical programs; red, explicitly labelled sections describe corrections. An appended cross-language chapter compares key C/Rust/Gforth/Factor snippets; a figure atlas embeds plots when generated first. Problems 13 and 17 now have C chapters too.

Pull-request validation checks helpers, builds optimized native code, profiles all 111 existing implementations plus 12 Factor scripts on one Linux runner, generates figures and compiles the exact LaTeX. A downloadable Actions artifact preserves measurements, plots, PDF and LaTeX sources for 30 days. There are no scheduled builds. `make clean` removes generated build outputs.

**Development status:** The established C/Rust/Gforth baseline is in `docs/VALIDATION.md`. New Factor source requires an independent passing CI run before claiming correctness or numeric performance. The 12/37 Factor subset is not included in any full-suite language ranking.
