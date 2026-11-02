#!/usr/bin/env python3
"""Regenerate the source index and new-solution method catalog from the manifest."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def answer_suffix(answer: str) -> str:
    """Show enough of a result to identify it without revealing the full answer."""
    return "…" + answer[-4:]


def main() -> None:
    manifest = json.loads((ROOT / "tests/answers.json").read_text())
    problems = sorted(manifest["problems"], key=lambda p: p["number"])
    first_120 = [p for p in problems if p["number"] <= 120]
    new = [p for p in first_120 if not p["covered_by_report"]]
    covered = [p["number"] for p in first_120 if p["covered_by_report"]]
    index = ["# Project Euler solution index", "",
        "C covers every official problem number from 1 through 255. The 37 solutions already",
        "addressed in the report are retained; another 218 have C implementations.", "",
        "Every C file contains its algorithm. The shared `support/c/euler_common.c`",
        "module provides reusable mathematical operations. Historical attempts are in",
        "`c/variants/`; they are not the programs built by the default target.", "",
        "Rust, Gforth, and Factor have implemented entry points through problem 120; later",
        "entries are C-only and have separate generated scaffolds for those languages.",
        "The implementation column records source coverage, not validation status.", "",
        "See [new C methods and bounds](../docs/C_1_120.md),",
        "[C validation](../docs/VALIDATION_C_1_120.md),",
        "[Gforth validation](../docs/VALIDATION_FORTH_1_120.md), and",
        "[the existing report](../explanations/latex/project_euler_c_explanations.tex).", "",
        "| Number | Problem | C source | Status | Other implementations |",
        "| ---: | --- | --- | --- | --- |"]
    for p in problems:
        other = [language for language in p["languages"] if language != "c"]
        index.append(f"| {p['number']} | [{p['title']}]({p['source']}) | "
                     f"[C](c/{p['stem']}.c) | "
                     f"{'Existing report' if p['covered_by_report'] else 'Added'} | "
                     f"{', '.join(other) if other else 'C only'} |")
    (ROOT / "solutions/README.md").write_text("\n".join(index) + "\n")

    notes = ["# C solutions through problem 120", "",
        "This addition implements the 83 problems missing from the existing 37-solution report.",
        "Every problem now has one title-named C program, ending in its official problem number.",
        "The source and report baseline is commit `" + manifest["baseline_commit"] + "`.", "",
        "## Scope", "",
        "The existing report's solution chapters, rather than its illustrative figure atlas,",
        "define the original exclusions. Those 37 algorithms now live in their individual",
        "C files; the shared module contains mathematical helpers only.", "",
        "Excluded problem numbers: " + ", ".join(map(str, covered)) + ".", "",
        "| Problem range | Existing report | New C programs | Total C programs |",
        "| --- | ---: | ---: | ---: |",
        "| 1–60 | 35 | 25 | 60 |",
        "| 61–90 | 1 | 29 | 30 |",
        "| 91–120 | 1 | 29 | 30 |",
        "| Total | 37 | 83 | 120 |", "",
        "## Build and run", "",
        "Run from the repository root. A C11 compiler, GMP development files, GNU Make,",
        "and Python 3 are sufficient for the C suite. The solvers do not use the network.", "",
        "```sh", "make -j4 c-release", "make test-c", "make unit-c",
        "make run-c PROBLEM=120", "make sanitize-c  # Linux: AddressSanitizer and UBSan", "```", "",
        "On Windows with GCC and GMP available through MSYS2/UCRT64, use",
        "`./scripts/test.ps1 -Action test-c`. Windows execution has not been verified in this change.", "",
        "`tests/answers.json` records reference answers and implementation coverage. The runner",
        "checks exact one-line stdout and a successful exit. It reports missing executables,",
        "wrong answers, errors and timeouts separately. Its timings include process startup;",
        "a single test run does not establish a statistical performance comparison.", "",
        "## Inputs", "",
        "Fifteen official resource files are included, with original byte counts, source URLs",
        "and SHA-256 hashes in `tests/data/manifest.json`. `make check-inputs` validates them",
        "offline. `make fetch-inputs` explicitly restores a missing or changed file, rejecting",
        "any download that differs from the pinned hash. Each solver also rejects missing",
        "or malformed input. The original report's datasets remain at their existing paths.", "",
        "## Arithmetic and search decisions", "",
        "The new programs use fixed-width integer types and GMP where native integers are too",
        "small. Exact square roots correct the floating initial estimate with integer division.",
        "The shared 64-bit primality test uses [Jim Sinclair's seven deterministic",
        "Miller–Rabin bases](https://miller-rabin.appspot.com/). Its native and portable modular",
        "multiplication paths are checked against GMP.", "",
        "Problem 60 first finds a feasible five-prime set, then searches every prime below",
        "that candidate sum. Every better set must occur in the second search. Problem 70",
        "sieves all eligible totients, with exact ratio comparisons. Problem 103 uses the",
        "statement's six-element set to construct an upper bound, then exhausts all smaller",
        "increasing seven-element candidates. Problems 108 and 110 enumerate nonincreasing",
        "prime-exponent vectors below a feasible primorial. These searches do not assume that",
        "a guessed local neighborhood contains the optimum.", "",
        "Problem 99 compares rational bounds on logarithms and falls back to exact powers if",
        "the bounds overlap. Problem 104 computes the full Fibonacci integer for every",
        "pandigital trailing-digit candidate, so a floating estimate cannot discard a match.", "",
        "### Two model qualifications", "",
        "Problem 59 exhausts every lowercase three-letter key and ranks the resulting text",
        "with an English-language score. That recognition rule is a documented heuristic;",
        "the official input produces the expected sum `" + answer_suffix("129448") + "`.", "",
        "Problem 84 solves a 30,720-state Markov chain with cyclic deck indices and the",
        "consecutive-doubles count. The initial shuffle is unspecified in the [official",
        "problem](https://projecteuler.net/problem=84), so the code declares two possible deck",
        "orders. Both give the requested four-sided modal string `" + answer_suffix("101524") + "`. Probabilities are",
        "conditional on those orders, rather than averaged over every possible shuffle.", "",
        "| Six-sided model | GO probability | Square 19 probability | Modal string |",
        "| --- | ---: | ---: | --- |",
        "| Explicit cyclic order 0 | 0.030976091750 | 0.031045560279 | `" + answer_suffix("102419") + "` |",
        "| Explicit cyclic order 1 | 0.030992122560 | 0.030852423967 | `" + answer_suffix("102400") + "` |", "",
        "This small difference explains why one fixed order differs from the statement's",
        "illustrative six-sided result. An independent rule interpreter checks every one of",
        "the 1,105,920 state/dice combinations. Regression tests retain both six-sided",
        "conditional results, the four-sided answer, and the card/jail edge cases.", "",
        "## The 83 new solutions", "",
        "These answer suffixes identify the automated checks without publishing full results.",
        "Each source computes its answer from the problem constraints or its official input; the table is not read by",
        "the solvers. Follow each C link for the recurrence, search bound, and implementation.", "",
        "| No. | Problem and source | Answer suffix | Method |",
        "| ---: | --- | ---: | --- |"]
    for p in new:
        notes.append(f"| {p['number']} | [{p['title']}](../solutions/c/{p['stem']}.c) | "
                     f"`{answer_suffix(p['answer'])}` | {p['method'].replace('|', '/')} |")
    notes += ["", "## Verification entry points", "",
        "- `make check-c-coverage` checks exactly 255 numbered sources and the 37 report exclusions.",
        "- `make check-inputs` checks all 15 official input hashes.",
        "- `make test-c` checks all 255 exact answers.",
        "- `make unit-c` runs independent examples, arithmetic boundaries, brute-force comparisons, and rule checks.",
        "- `make unit-python` checks the runner, language coverage, and existing visualization arithmetic.",
        "- `make sanitize-c` compiles and executes all 255 programs with AddressSanitizer and UBSan on Linux.", "",
        "- `make audit-c` independently checks expression trees, letter mappings, repeated primes, and digit-set partitions.", "",
        "The [validation record](VALIDATION_C_1_120.md) distinguishes what was executed from",
        "what is configured for future CI runs. Regenerate this catalog with `make catalog-c`."]
    (ROOT / "docs/C_1_120.md").write_text("\n".join(notes) + "\n")
    print(f"Generated source index for {len(problems)} problems and 1–120 methods for {len(new)} additions")


if __name__ == "__main__":
    main()
