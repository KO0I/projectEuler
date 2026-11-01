#!/usr/bin/env python3
"""Regenerate runtime comparison charts from CSV; never rerun solutions."""
from __future__ import annotations
import csv
import math
from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / "build" / "profile"
OUT = DATA / "plots"
LANGUAGES = ("c", "rust", "forth")
COLORS = {"c": "#2b6684", "rust": "#cc7954", "forth": "#76718c"}
HATCHES = {"c": "///", "rust": "\\\\\\", "forth": "xxx"}


def load():
    results = {}
    for language in LANGUAGES:
        path = DATA / f"{language}.csv"
        if not path.is_file():
            continue
        with path.open(newline="", encoding="utf-8") as handle:
            results[language] = {int(row["problem"]): row for row in csv.DictReader(handle)}
    return results


def write(fig, stem):
    OUT.mkdir(parents=True, exist_ok=True)
    for ext in ("svg", "png", "pdf"):
        fig.savefig(OUT / f"{stem}.{ext}", dpi=180, bbox_inches="tight")
    plt.close(fig)


def runtime_by_problem(rows):
    numbers = sorted(set().union(*(set(d) for d in rows.values())))
    if not numbers:
        return
    fig, ax = plt.subplots(figsize=(17, 6))
    width = 0.26
    for i, language in enumerate(LANGUAGES):
        if language not in rows:
            continue
        durations = []
        for n in numbers:
            r = rows[language].get(n)
            durations.append(float(r["runtime_ms"])
                             if r and r["status"] == "pass" and r["runtime_ms"]
                             else math.nan)
        xs = np.arange(len(numbers)) + (i - 1) * width
        ax.bar(xs, durations, width, label=language.upper(),
               color=COLORS[language], hatch=HATCHES[language],
               edgecolor="#222222", linewidth=.5)
        # Explicit missing/failed marks, positioned above the lower log limit.
        for x, n, duration in zip(xs, numbers, durations):
            if not math.isfinite(duration):
                ax.plot(x, 0.5, "x", color="#222222", markersize=4)
    ax.set_yscale("log")
    ax.set_ylim(bottom=0.3)
    ax.set_xticks(np.arange(len(numbers)), [str(n) for n in numbers])
    ax.set_xlabel("Project Euler problem number")
    ax.set_ylabel("Single-run wall-clock duration [ms] (log scale)")
    ax.set_title("Runtime by problem and language (one execution; compilation excluded)")
    ax.grid(axis="y", linestyle=":", alpha=.45)
    ax.legend()
    ax.text(.005, -.18, "X = unavailable, failed, or untested; no error bars are inferred from one run.",
            transform=ax.transAxes, fontsize=9)
    fig.tight_layout()
    write(fig, "runtime_by_problem")


def totals(rows):
    if not rows:
        return
    fig, ax = plt.subplots(figsize=(8, 5))
    for i, language in enumerate(LANGUAGES):
        if language not in rows:
            continue
        entries = list(rows[language].values())
        passed = [r for r in entries if r["status"] == "pass" and r["runtime_ms"]]
        total = sum(float(r["runtime_ms"]) for r in passed)
        ax.bar(i, total, color=COLORS[language],
               hatch=HATCHES[language], edgecolor="#222222")
        label = f"{len(passed)}/{len(entries)} passing"
        if passed:
            ax.annotate(label, (i, total), xytext=(0, 5), textcoords="offset points",
                        ha="center", va="bottom", fontsize=10)
        else:
            ax.annotate(label, (i, 0), xytext=(0, 5), textcoords="offset points",
                        ha="center", fontsize=10)
    ax.set_xticks(range(3), [l.upper() for l in LANGUAGES])
    ax.set_ylabel("Sum of passing child process runtimes [ms]")
    ax.set_title("Total measured runtime by language")
    ax.text(.5, -.2, "Partial totals are NOT comparable to complete 37-problem totals.",
            transform=ax.transAxes, ha="center", fontsize=9)
    ax.grid(axis="y", linestyle=":", alpha=.4)
    fig.tight_layout()
    write(fig, "totals")


if __name__ == "__main__":
    results = load()
    if not results:
        raise SystemExit("No CSV measurements under build/profile; run make profile first.")
    runtime_by_problem(results)
    totals(results)
    print(f"Saved SVG/PNG/PDF charts under {OUT}")
