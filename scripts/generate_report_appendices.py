#!/usr/bin/env python3
"""Generate source-backed LaTeX reference sections for the Euler report.

The generated files are intentionally mechanical.  Handwritten chapters explain
the mathematics in depth; these sections guarantee that every declared problem
and every shared helper has a current, navigable report entry.
"""
from __future__ import annotations

import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "explanations" / "latex" / "generated"
ANSWERS = ROOT / "tests" / "answers.json"

LANGUAGES = ("c", "rust", "forth", "factor")
DISPLAY = {"c": "C", "rust": "Rust", "forth": "Gforth", "factor": "Factor"}


def tex(value: str) -> str:
    """Escape prose for LaTeX without changing its words."""
    replacements = {
        "\\": r"\textbackslash{}", "&": r"\&", "%": r"\%",
        "$": r"\$", "#": r"\#", "_": r"\_", "{": r"\{",
        "}": r"\}", "~": r"\textasciitilde{}", "^": r"\textasciicircum{}",
    }
    return "".join(replacements.get(ch, ch) for ch in value)


def code(value: str) -> str:
    """Format a short symbol or path without LaTeX escaping its punctuation."""
    return r"\texttt{\detokenize{" + value.replace("}", "") + "}}"


def path_code(value: str) -> str:
    """Format a path with legal line breaks at directory separators."""
    return r"\path{" + value.replace("}", "") + "}"


def relative(path: Path) -> str:
    return path.relative_to(ROOT).as_posix()


def source_path(problem: dict, language: str) -> Path:
    number = problem["number"]
    if language == "c":
        return ROOT / "solutions" / "c" / f"{problem['stem']}.c"
    if language == "rust":
        return ROOT / "solutions" / "rust" / "src" / "bin" / f"p{number}.rs"
    if language == "forth":
        return ROOT / "solutions" / "forth" / f"p{number}.fs"
    return ROOT / "solutions" / "factor" / f"p{number}.factor"


def source_material(problem: dict, language: str) -> tuple[Path, str]:
    """Return the file that owns the algorithm, not a thin Factor launcher."""
    entry = source_path(problem, language)
    text = entry.read_text(encoding="utf-8")
    if language != "factor":
        return entry, text
    number = problem["number"]
    definition = re.compile(rf"^::?\s+p{number}\s+\(", re.M)
    for candidate in sorted((ROOT / "solutions/factor/euler").glob("**/*.factor")):
        candidate_text = candidate.read_text(encoding="utf-8")
        match = definition.search(candidate_text)
        if match:
            next_definition = re.search(r"^::?\s+\S+\s+\(", candidate_text[match.end():], re.M)
            end = match.end() + next_definition.start() if next_definition else len(candidate_text)
            return candidate, candidate_text[match.start():end].rstrip()
    return entry, text


def leading_explanation(text: str, language: str) -> str:
    """Use the source owner's opening comment as the compact strategy note."""
    if language == "c":
        match = re.match(r"\s*/\*(.*?)\*/", text, re.S)
        raw = match.group(1) if match else ""
        lines = [re.sub(r"^\s*\*\s?", "", line).strip() for line in raw.splitlines()]
    elif language == "rust":
        lines = []
        for line in text.splitlines():
            if line.startswith("//!"):
                lines.append(line[3:].strip())
            elif lines:
                break
    else:
        marker = "\\" if language == "forth" else "!"
        lines = [line.lstrip(marker).strip() for line in text.splitlines()
                 if line.strip().startswith(marker)]
    lines = [line for line in lines
             if line and not line.startswith("Project Euler ")
             and not re.match(r"https?://", line)]
    prose = " ".join(lines)
    prose = re.sub(r"https?://\S+", "", prose)
    prose = prose.replace("This file owns the problem algorithm.", "")
    return re.sub(r"\s+", " ", prose).strip()


def snippet(text: str, language: str, number: int, limit: int = 14) -> str:
    """Select a short algorithm-bearing excerpt instead of file boilerplate."""
    lines = text.splitlines()
    starts = {
        "c": re.compile(r"^(static |int main|typedef |enum )"),
        "rust": re.compile(r"^(fn |const |struct |enum |#\[derive)"),
        "forth": re.compile(r"^:\s|^:\S|^create\s|^variable\s"),
        "factor": re.compile(rf"^::?\s+p{number}\s+\(|^p{number}\s+number>string\s+print"),
    }
    start = next((i for i, line in enumerate(lines) if starts[language].search(line)), 0)
    selected = []
    for line in lines[start:]:
        if not line.strip() and not selected:
            continue
        selected.append(line.rstrip())
        if len(selected) >= limit:
            break
    excerpt = "\n".join(selected).strip()
    # pdfLaTeX's listings package accepts ASCII reliably but rejects several
    # Unicode math glyphs found in source comments.  Keep the mathematical
    # meaning where a short ASCII spelling exists, then expose any remaining
    # code point as a readable \uNNNN escape.
    translations = str.maketrans({
        "²": "^2", "³": "^3", "×": "x", "≤": "<=", "≥": ">=",
        "−": "-", "–": "-", "—": "-", "…": "...", "√": "sqrt",
        "φ": "phi", "π": "pi", "Σ": "sum", "Δ": "delta",
    })
    excerpt = excerpt.translate(translations)
    return excerpt.encode("ascii", "backslashreplace").decode("ascii")


FEATURES = (
    ("prime generation or testing", re.compile(r"prime|sieve", re.I)),
    ("modular arithmetic", re.compile(r"powmod|mod_pow|modular-pow|\bmod\b", re.I)),
    ("dynamic programming or memoization", re.compile(r"memo|cache|\bdp\b|dynamic", re.I)),
    ("exact large-integer arithmetic", re.compile(r"mpz|BigUint|bigint|bnew|factorial", re.I)),
    ("fixture parsing", re.compile(r"read-|read_|file-|file_|include_str", re.I)),
    ("sorting or set deduplication", re.compile(r"sort|HashSet|BTreeSet|assoc|set-at", re.I)),
    ("recursive search", re.compile(r"recurse|recursive|fn search|: search", re.I)),
)


def features(text: str) -> list[str]:
    return [label for label, pattern in FEATURES if pattern.search(text)]


def join_words(items: list[str]) -> str:
    if not items:
        return "direct bounded arithmetic"
    if len(items) == 1:
        return items[0]
    return ", ".join(items[:-1]) + ", and " + items[-1]


def extract_names(paths: list[Path], pattern: re.Pattern[str]) -> set[str]:
    names: set[str] = set()
    for path in paths:
        names.update(pattern.findall(path.read_text(encoding="utf-8")))
    return names


def api_names() -> dict[str, set[str]]:
    c_paths = [ROOT / "support/c/euler_common.h", ROOT / "support/c/euler_extra.h"]
    rust_paths = [ROOT / "solutions/rust/src/extra_helpers.rs"]
    forth_paths = [ROOT / "solutions/forth/math.fs", ROOT / "solutions/forth/big.fs"]
    factor_paths = [ROOT / "solutions/factor/euler/shared/shared.factor"]
    return {
        "c": extract_names(c_paths, re.compile(
            r"^(?:static inline\s+)?(?:void|bool|int|unsigned(?:\s+char)?|"
            r"long\s+long|uint64_t|FILE)\s*\**\s*(\w+)\s*\([^;{]*\)\s*[;{]",
            re.M)),
        "rust": extract_names(rust_paths, re.compile(r"^pub fn\s+(\w+)", re.M)),
        "forth": extract_names(forth_paths, re.compile(r"^:\s+(\S+)", re.M)),
        "factor": extract_names(factor_paths, re.compile(r"^::?\s+(\S+)", re.M)),
    }


def used_helpers(text: str, names: set[str], language: str) -> list[str]:
    if language in {"c", "rust"}:
        local = set(re.findall(
            r"^(?:pub\s+)?(?:static\s+)?(?:fn\s+|[\w* ]+\s)(\w+)\s*\(",
            text, re.M))
        return sorted(name for name in names - local
                      if re.search(rf"(?<![\w-]){re.escape(name)}\s*\(", text))
    return sorted(name for name in names
                  if re.search(rf"(?<![\w-]){re.escape(name)}(?![\w-])", text))


def generate_walkthroughs(problems: list[dict]) -> str:
    helpers = api_names()
    output = [
        r"\section{Compact walkthroughs for problems 1--255}",
        r"\label{sec:allwalkthroughs}",
        "",
        "This section is generated from the answer manifest and the current source files. "
        "Each entry records the implementation strategy, shows a short excerpt from every "
        "available language, and names any shared helper calls. The excerpt is an entry point "
        "into the source, not a replacement for the complete program.",
        "",
    ]
    for problem in problems:
        number = problem["number"]
        available = [lang for lang in LANGUAGES if lang in problem["languages"]]
        material = {lang: source_material(problem, lang) for lang in available}
        sources = {lang: value[0] for lang, value in material.items()}
        texts = {lang: value[1] for lang, value in material.items()}
        owner = "c" if "c" in texts else available[0]
        strategy = leading_explanation(texts[owner], owner)
        if not strategy:
            strategy = f"The program computes the exact answer for {problem['title']}."

        output.extend((
            rf"\subsection{{Problem {number}: {tex(problem['title'])}}}",
            rf"\label{{walkthrough:p{number}}}",
            tex(strategy),
            "",
        ))
        for language in available:
            path = sources[language]
            output.extend((
                rf"\paragraph{{{DISPLAY[language]}.}} Source: {path_code(relative(path))}.",
                r"\begin{lstlisting}",
                snippet(texts[language], language, number),
                r"\end{lstlisting}",
                "",
            ))

        missing = [DISPLAY[lang] for lang in LANGUAGES if lang not in available]
        output.append(r"\paragraph{Comparison.}")
        per_language = []
        for language in available:
            signals = join_words(features(texts[language]))
            calls = used_helpers(texts[language], helpers[language], language)
            call_note = (" Shared calls: " + ", ".join(code(name) for name in calls) + ".") if calls else ""
            per_language.append(f"{DISPLAY[language]} exposes {signals}.{call_note}")
        output.append(" ".join(per_language))
        if missing:
            output.append("No validated source exists for " + tex(join_words(missing)) + ".")
        output.extend((
            "All listed programs print the same exact answer string from the manifest. "
            "The excerpts can differ in algorithm, data representation, and startup cost.",
            "",
        ))
    return "\n".join(output) + "\n"


PURPOSES = {
    "gcd": "Computes a greatest common divisor.",
    "integer-gcd": "Computes a greatest common divisor.",
    "euler_gcd": "Computes a greatest common divisor.",
    "prime": "Tests primality by trial division.",
    "prime64": "Tests every 64-bit input with deterministic Miller--Rabin witnesses.",
    "euler_is_prime": "Tests every 64-bit input with deterministic Miller--Rabin witnesses.",
    "is-prime?": "Tests primality by trial division.",
    "sieve": "Builds primality flags through an inclusive bound.",
    "euler_sieve": "Builds primality flags through an inclusive bound.",
    "sieve-build": "Builds primality flags through an inclusive bound.",
    "euler-sieve": "Builds primality flags through an inclusive bound.",
    "isqrt": "Returns the floor of an integer square root.",
    "integer-sqrt": "Returns the floor of an integer square root.",
    "euler_isqrt": "Returns the floor of an integer square root.",
    "euler-isqrt": "Returns the floor of an integer square root.",
    "choose": "Computes a binomial coefficient exactly.",
    "mod_pow": "Computes an integer power modulo a modulus.",
    "euler_powmod": "Computes an integer power modulo a modulus.",
    "power-mod": "Computes an integer power modulo a modulus.",
    "modular-pow": "Computes an integer power modulo a modulus.",
}


def purpose(name: str) -> str:
    if name in PURPOSES:
        return PURPOSES[name]
    if "read" in name or "parse" in name or "fixture" in name or "matrix" in name:
        return "Reads or parses a committed problem fixture."
    if "digit" in name or "decimal" in name or "pandigital" in name:
        return "Provides a decimal-digit operation."
    if "divisor" in name or "factor" in name or "totient" in name or "phi" in name:
        return "Provides a number-theory operation."
    if name.startswith("b") and name not in {"bit-count", "big_isqrt"}:
        return "Provides base-10000 arbitrary-precision storage or arithmetic."
    if "triangle" in name or "path" in name:
        return "Provides a combinatoric or path operation."
    if "alloc" in name or "fail" in name or "open" in name:
        return "Checks a C allocation, file, or failure boundary."
    return "Provides a reusable mathematical operation."


def api_entries() -> dict[str, list[tuple[str, str, str]]]:
    entries: dict[str, list[tuple[str, str, str]]] = {lang: [] for lang in LANGUAGES}
    c_text = "\n".join((ROOT / "support/c/euler_common.h").read_text().splitlines() +
                       (ROOT / "support/c/euler_extra.h").read_text().splitlines())
    c_pattern = re.compile(
        r"^(?:static inline\s+)?(?:void|bool|int|unsigned(?:\s+char)?|"
        r"long\s+long|uint64_t|FILE)\s*\**\s*(\w+)\s*\([^;{]*\)\s*[;{]",
        re.M)
    for match in c_pattern.finditer(c_text):
        signature = re.sub(r"\s+", " ", match.group(0).rstrip(";{").strip())
        entries["c"].append((match.group(1), signature, purpose(match.group(1))))

    rust_text = (ROOT / "solutions/rust/src/extra_helpers.rs").read_text()
    for match in re.finditer(r"^pub fn\s+(\w+)[^{]+", rust_text, re.M):
        signature = re.sub(r"\s+", " ", match.group(0).strip())
        entries["rust"].append((match.group(1), signature, purpose(match.group(1))))

    for path in (ROOT / "solutions/forth/math.fs", ROOT / "solutions/forth/big.fs"):
        for line in path.read_text().splitlines():
            match = re.match(r"^:\s+(\S+).*?(\([^)]*\))", line)
            if match:
                entries["forth"].append((match.group(1), match.group(2), purpose(match.group(1))))

    factor_path = ROOT / "solutions/factor/euler/shared/shared.factor"
    for line in factor_path.read_text().splitlines():
        match = re.match(r"^::?\s+(\S+)\s+(\([^)]*\))", line)
        if match:
            entries["factor"].append((match.group(1), match.group(2), purpose(match.group(1))))
    return entries


def generate_api() -> str:
    origins = {
        "c": "support/c/euler_common.h and support/c/euler_extra.h",
        "rust": "solutions/rust/src/extra_helpers.rs",
        "forth": "solutions/forth/math.fs and solutions/forth/big.fs",
        "factor": "solutions/factor/euler/shared/shared.factor",
    }
    output = [
        r"\section{Shared Project Euler library API}",
        r"\label{sec:libraryapi}",
        "",
        "This appendix is a generated reference. The problem programs own their algorithms. "
        "These modules provide arithmetic, input, and collection operations that more than one "
        "solver can reuse. Stack comments are the signatures for Gforth and Factor words.",
        "",
    ]
    for language, entries in api_entries().items():
        output.extend((
            rf"\subsection{{{DISPLAY[language]} API}}",
            "Source: " + ", ".join(path_code(item.strip()) for item in origins[language].split(" and ")) + ".",
            r"\begin{description}",
        ))
        seen = set()
        for name, signature, note in entries:
            if name in seen:
                continue
            seen.add(name)
            output.append(rf"\item[{code(name)}] {code(signature)}. {note}")
        output.extend((r"\end{description}", ""))
    return "\n".join(output) + "\n"


def main() -> None:
    manifest = json.loads(ANSWERS.read_text(encoding="utf-8"))
    problems = manifest["problems"]
    if [problem["number"] for problem in problems] != list(range(1, 256)):
        raise SystemExit("tests/answers.json must contain contiguous problems 1 through 255")
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / "problem_walkthroughs.tex").write_text(generate_walkthroughs(problems), encoding="utf-8")
    (OUT / "library_api.tex").write_text(generate_api(), encoding="utf-8")
    print(f"Generated {len(problems)} walkthroughs and the shared-library API in {OUT}")


if __name__ == "__main__":
    main()
