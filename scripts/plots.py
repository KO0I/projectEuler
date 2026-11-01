#!/usr/bin/env python3
"""Regenerate accessible SVG, PNG and PDF charts from ONE saved profile run."""
from __future__ import annotations
import argparse
import csv
import json
import math
from pathlib import Path
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import numpy as np

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "build" / "profile"
COLORS = {"c": "#2b6684", "rust": "#cc7954", "forth": "#76718c", "factor": "#407c61"}
HATCHES = {"c": "///", "rust": "\\\\\\", "forth": "xxx", "factor": "oo"}


def load(run: Path | None = None):
    if run is None:
        pointer = BASE / "latest.json"
        if not pointer.is_file():
            raise ValueError("No profile exists. Run make profile first.")
        run = BASE / json.loads(pointer.read_text())["run_id"]
    info = json.loads((run / "metadata.json").read_text(encoding="utf-8"))
    if info.get("schema") != 2 or info.get("mode") != "profile":
        raise ValueError("Expected a schema-2 profiling run, not test output")
    rows = {}
    for language in info["languages"]:
        with (run / f"{language}.csv").open(newline="", encoding="utf-8") as handle:
            records = list(csv.DictReader(handle))
        numbers = [int(r["problem"]) for r in records]
        if sorted(numbers) != sorted(info.get("coverage", {}).get(language, info["problems"])):
            raise ValueError(f"Missing or duplicated problem rows for {language}")
        for row in records:
            if row["run_id"] != info["run_id"] or row["language"] != language:
                raise ValueError("Refusing to combine observations from different runs")
            if row["status"] == "pass":
                ms = float(row["runtime_ms"])
                if not math.isfinite(ms) or ms <= 0:
                    raise ValueError("Passing observations need a positive measured duration")
        rows[language] = {int(r["problem"]): r for r in records}
    return run, info, rows


def write(fig, out: Path, stem: str):
    out.mkdir(parents=True, exist_ok=True)
    for ext in ("svg", "png", "pdf"):
        fig.savefig(out / f"{stem}.{ext}", dpi=180, bbox_inches="tight")
    plt.close(fig)


def runtime_by_problem(rows, info, out):
    numbers = sorted(info["problems"])
    languages = info["languages"]
    fig, ax = plt.subplots(figsize=(17, 6))
    width = 0.8 / len(languages)
    positive = []
    for index, language in enumerate(languages):
        times = [float(rows[language][n]["runtime_ms"])
                 if n in rows[language] and rows[language][n]["status"] == "pass"
                 else math.nan for n in numbers]
        positive.extend(ms for ms in times if math.isfinite(ms))
        xs = np.arange(len(numbers)) + (index - (len(languages) - 1) / 2) * width
        ax.bar(xs, times, width, label=language.upper(), color=COLORS[language],
               hatch=HATCHES[language], edgecolor="#222222", linewidth=.5)
        for x, ms, number in zip(xs, times, numbers):
            if not math.isfinite(ms):
                # Hollow circle: not implemented; X: attempted and failed.
                absent = number not in rows[language]
                ax.plot(x, .02, "o" if absent else "x",
                        transform=ax.get_xaxis_transform(),
                        color="#222222",
                        markerfacecolor="none" if absent else "#222222",
                        markersize=4 if absent else 5)
    if positive:
        ax.set_yscale("log")
        ax.set_ylim(bottom=min(positive) / 2, top=max(positive) * 1.5)
    ax.set_xticks(np.arange(len(numbers)), [str(n) for n in numbers])
    ax.set_xlabel("Project Euler problem number")
    ax.set_ylabel("Single-run wall-clock duration [ms]" + (" (log scale)" if positive else ""))
    ax.set_title("Runtime by problem and language — one execution per program")
    ax.grid(axis="y", linestyle=":", alpha=.4)
    ax.legend()
    fig.text(.01, .015, "X = failed/unavailable; hollow circle = not implemented. Neither means zero [ms]. "
             "Compilation excluded; startup included. No single-run error bars.", fontsize=9)
    fig.text(.01, -.015, f"Run: {info['run_id']} | revision: {info['environment']['revision'][:12]}", fontsize=8)
    fig.tight_layout(rect=(0, .06, 1, 1))
    write(fig, out, "runtime_by_problem")


def totals(rows, info, out):
    fig, ax = plt.subplots(figsize=(9, 6))
    languages = info["languages"]
    for i, language in enumerate(languages):
        summary = info["language_summaries"][language]
        count = len(rows[language])
        complete = (summary["complete"] and summary.get("full_coverage", True)
                    and len(rows[language]) == len(info["problems"])
                    and all(r["status"] == "pass" for r in rows[language].values()))
        if not complete:
            status = "Partial coverage" if not summary.get("full_coverage", True) else "Incomplete"
            ax.text(i, .04, f"{status}\n{summary['passed']}/{count} pass\n"
                    f"{count}/{len(info['problems'])} implemented",
                    ha="center", transform=ax.get_xaxis_transform())
            continue
        wall = summary["suite_wall_ms"]
        child = summary["passing_child_runtime_ms"]
        ax.bar(i, wall, color=COLORS[language], hatch=HATCHES[language],
               edgecolor="#222222", width=.62)
        ax.plot(i, child, "D", color="white", markeredgecolor="black", markersize=6)
        ax.annotate(f"{wall:.2f} [ms]\n{count}/{count} pass", (i, wall),
                    xytext=(0, 6), textcoords="offset points", ha="center", fontsize=10)
    ax.set_xticks(range(len(languages)), [l.upper() for l in languages])
    ax.set_ylabel("Sequential suite wall-clock duration [ms]")
    ax.set_title("Complete-suite runtime by language — same run and machine")
    ax.margins(y=.25)
    ax.grid(axis="y", linestyle=":", alpha=.4)
    ax.legend(handles=[Line2D([], [], linestyle="none", marker="D", color="white",
                              markeredgecolor="black", label="Sum of child-process durations")])
    fig.text(.05, .03, "Bars include runner overhead; diamonds exclude it. Partial/failed suites have no total bar.\n"
             "Algorithms, libraries and fixture loading differ: these are implementation timings, not language rankings.",
             fontsize=9)
    fig.tight_layout(rect=(0, .1, 1, 1))
    write(fig, out, "totals")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--run", type=Path, help="Saved run directory; default is the latest profile")
    args = parser.parse_args()
    try:
        run, info, rows = load(args.run)
    except (OSError, ValueError, KeyError) as exc:
        parser.error(str(exc))
    runtime_by_problem(rows, info, run / "plots")
    totals(rows, info, run / "plots")
    print(f"Saved SVG/PNG/PDF charts under {run / 'plots'}")


if __name__ == "__main__":
    main()
