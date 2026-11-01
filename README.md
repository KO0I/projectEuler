# Project Euler

Historical Project Euler solutions and experiments, arranged by language.

- [Problem index](solutions/README.md)
- [C source explanations in LaTeX](explanations/latex/README.md)
- [Combined LaTeX explanations](explanations/latex/project_euler_c_explanations.tex)
- [Original-to-new path map](docs/renames.tsv)

## Layout

- `solutions/c/`: C solutions and unfinished attempts
- `solutions/shell/`: shell scripts
- `solutions/mathematica/`: Mathematica notebooks
- `solutions/latex/`: LaTeX write-ups and existing PDFs
- `explanations/latex/`: commentary on each main C implementation, in problem order
- `support/c/`: shared routines and experimental tools
- `archive/`: historical binaries, build artifacts and old notes

Main solution filenames follow the official Project Euler title, with the problem number last before the extension. Variants and historical copies are preserved in `variants/`.

## Build

From the repository root, run `make`, `make run`, or `make clean`. The default target remains problem 2, built into `build/`. To compile other standalone programs, use `cc -std=c99 solutions/c/multiples_of_3_or_5_1.c -o build/problem1` after creating `build/`.

For the combined LaTeX explanations, run `make -C explanations/latex`. This requires a LaTeX installation with pdflatex and standard support packages.

Some C programs need `-lm`, GNU MP (`-lgmp`), or additional C support files. Some source files are unfinished or contain bugs. Existing data files remain at their original paths to preserve relative-path input reads.

## Three-language corrected implementations

The development branch covers all 37 represented problems in C, Rust and Forth. Corrected C wrappers call `support/c/euler_common.c`; earlier source remains under `solutions/c/variants/historical/`. Rust uses a Cargo package under `solutions/rust/`, while Forth uses Gforth and reusable base-10000 arithmetic in `solutions/forth/`.

With the required toolchains installed, run `make test` to check answers, `make profile` for one-run timing and plots, and `make plots-math` for mathematical figures. See `docs/adr/0001-c-rust-forth.md`. LaTeX explanations mark corrections in red and explicitly label them for monochrome readability.
