# Project Euler

Historical Project Euler solutions and experiments, arranged by language.

- [Problem index](solutions/README.md)
- [Original-to-new path map](docs/renames.tsv)

## Layout

- `solutions/c/`: C solutions and unfinished attempts
- `solutions/shell/`: shell scripts
- `solutions/mathematica/`: Mathematica notebooks
- `solutions/latex/`: LaTeX write-ups and existing PDFs
- `support/c/`: shared routines and experimental tools
- `archive/`: historical binaries, build artifacts and old notes

Main solution filenames follow the official Project Euler title, with the problem number last before the extension. Variants and historical copies are preserved in `variants/`.

## Build

From the repository root, run `make`, `make run`, or `make clean`. The default target remains problem 2, built into `build/`. To compile other standalone programs, use `cc -std=c99 solutions/c/multiples_of_3_or_5_1.c -o build/problem1` after creating `build/`.

Some programs need `-lm`, GNU MP (`-lgmp`), or additional C support files. Some source files are unfinished or contain bugs. Existing data files remain at their original paths to preserve relative-path input reads.
