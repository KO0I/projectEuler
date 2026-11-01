#!/usr/bin/env python3
"""Regenerate accessible SVG, PNG and PDF charts from ONE saved profile run.

Where calibrated, gray hatched startup and colored execution-residual segments
add up to the *observed* child process wall time; a dark line marks the split.
The startup baseline is only a minimal same-language no-op estimate.
"""
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
from matplotlib.patches import Patch
import numpy as np

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "build" / "profile"
COLORS = {"c": "#2b6684", "rust": "#cc7954", "forth": "#76718c", "factor": "#407c61"}
HATCHES = {"c": "...", "rust": "\\\\\\", "forth": "xxx", "factor": "oo"}
STARTUP_COLOR = "#d2d2d2"
STARTUP_HATCH = "/////"
OVERHEAD_COLOR = "#fafafa"
OVERHEAD_HATCH = ".."
EDGE = "#202020"


def parts(row: dict) -> tuple[float, float | None, float | None]:
    """Return (actual wall, estimated startup, execution residual).

    Legacy (schema-2) or uncalibrated rows remain undivided, not inferred.
    """
    total = float(row["runtime_ms"])
    startup_raw, execution_raw = row.get("startup_estimate_ms"), row.get("execution_residual_ms")
    if not startup_raw or not execution_raw:
        return total, None, None
    if row.get("split_status") not in ("estimated", "capped"):
        raise ValueError("Startup/execute split needs an explicit estimation status")
    startup, execution = float(startup_raw), float(execution_raw)
    if not all(math.isfinite(v) for v in (total, startup, execution)) \
            or startup <= 0 or execution < 0 or abs(startup + execution - total) > 0.000002:
        raise ValueError("Startup + execution must match the observed positive wall duration")
    return total, startup, execution


def load(run: Path | None = None):
    if run is None:
        pointer = BASE / "latest.json"
        if not pointer.is_file():
            raise ValueError("No profile exists. Run make profile first.")
        run = BASE / json.loads(pointer.read_text())["run_id"]
    info = json.loads((run / "metadata.json").read_text(encoding="utf-8"))
    if info.get("schema") not in (2, 3) or info.get("mode") != "profile":
        raise ValueError("Expected a schema-2 or schema-3 profiling run, not test output")
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
                parts(row)  # Validate claimed decomposition before producing figures.
        rows[language] = {int(r["problem"]): r for r in records}
    return run, info, rows


def write(fig, out: Path, stem: str):
    out.mkdir(parents=True, exist_ok=True)
    for ext in ("svg", "png", "pdf"):
        fig.savefig(out / f"{stem}.{ext}", dpi=180, bbox_inches="tight")
    plt.close(fig)


def mark_split(ax, x: float, width: float, startup: float):
    """Strong dark boundary line survives grayscale and patterned printing."""
    if startup > 0:
        ax.hlines(startup, x - width * .5, x + width * .5,
                  colors=EDGE, linewidth=1.35, zorder=5)


def draw_timed_bar(ax, x: float, width: float, total: float,
                   startup: float | None, execution: float | None,
                   language: str, *, extras: float = 0.0):
    """Draw contiguous time portions with additive heights and fixed hatching."""
    if startup is None or execution is None:
        ax.bar(x, total + extras, width, color=COLORS[language],
               hatch=HATCHES[language], edgecolor=EDGE, linewidth=.6)
        return
    ax.bar(x, startup, width, color=STARTUP_COLOR, hatch=STARTUP_HATCH,
           edgecolor=EDGE, linewidth=.6)
    ax.bar(x, execution, width, bottom=startup, color=COLORS[language],
           hatch=HATCHES[language], edgecolor=EDGE, linewidth=.6)
    mark_split(ax, x, width, startup)
    if extras > 0:
        ax.bar(x, extras, width, bottom=startup + execution,
               color=OVERHEAD_COLOR, hatch=OVERHEAD_HATCH,
               edgecolor=EDGE, linewidth=.6)


def legend_handles(languages, *, overhead: bool = False, unattributed: bool = False):
    handles = [Patch(facecolor=STARTUP_COLOR, edgecolor=EDGE, hatch=STARTUP_HATCH,
                     label="Startup: no-op baseline estimate")]
    handles.extend(Patch(facecolor=COLORS[lang], edgecolor=EDGE,
                         hatch=HATCHES[lang], label=f"{lang.upper()} execution residual")
                   for lang in languages)
    if overhead:
        handles.append(Patch(facecolor=OVERHEAD_COLOR, hatch=OVERHEAD_HATCH,
                             edgecolor=EDGE, label="Runner overhead (suite only)"))
    if unattributed:
        handles.append(Patch(facecolor="white", hatch="---", edgecolor=EDGE,
                             label="Undivided: no startup calibration"))
    handles.append(Line2D([], [], color=EDGE, linewidth=1.35,
                          label="Startup / execution boundary"))
    return handles


def available_medians(info):
    medians = []
    for language in info["languages"]:
        entry = info.get("startup_calibrations", {}).get(language, {})
        if entry.get("status") == "measured":
            medians.append(f"{language.upper()} {entry['baseline_ms']:.3f} [ms]")
        else:
            medians.append(f"{language.upper()} unavailable")
    return "Minimal startup medians: " + "; ".join(medians)


def runtime_by_problem(rows, info, out):
    numbers = sorted(info["problems"])
    languages = info["languages"]
    fig, ax = plt.subplots(figsize=(18, 7))
    width = 0.8 / len(languages)
    positive = []
    unattributed = False
    for index, language in enumerate(languages):
        xs = np.arange(len(numbers)) + (index - (len(languages) - 1) / 2) * width
        for x, number in zip(xs, numbers):
            row = rows[language].get(number)
            if row and row["status"] == "pass":
                total, startup, execution = parts(row)
                positive.append(total)
                if startup is not None:
                    positive.append(startup)
                else:
                    unattributed = True
                draw_timed_bar(ax, x, width, total, startup, execution, language)
            else:
                # Hollow circle: not implemented; X: attempted and failed.
                absent = row is None
                ax.plot(x, .02, "o" if absent else "x",
                        transform=ax.get_xaxis_transform(), color=EDGE,
                        markerfacecolor="none" if absent else EDGE,
                        markersize=4 if absent else 5)
    if positive:
        ax.set_yscale("log")
        ax.set_ylim(bottom=min(positive) / 1.75, top=max(positive) * 2)
    ax.set_xticks(np.arange(len(numbers)), [str(n) for n in numbers])
    if len(numbers) > 50:
        ax.tick_params(axis="x", labelrotation=90, labelsize=7)
    ax.set_xlabel("Project Euler problem number")
    ax.set_ylabel("One measured child process [ms] (log scale)")
    ax.set_title("Startup baseline and appended execution time by Euler problem")
    ax.grid(axis="y", linestyle=":", alpha=.4)
    ax.legend(handles=legend_handles(languages, unattributed=unattributed), ncol=3,
              fontsize=8, loc="upper left")
    fig.text(.01, .060, available_medians(info), fontsize=9)
    fig.text(.01, .040, "Gray hatched base = estimated minimum startup; colored section = residual to observed total. "
             "Black line = split. X = failed; hollow circle = not implemented.", fontsize=9)
    fig.text(.01, .020, "Not exact per-solver startup: solver loading and I/O are in the residual. "
             "No warm-up or solver repeats; compilation excluded; no statistical error bars.", fontsize=8)
    fig.text(.01, .004, f"Run: {info['run_id']} | revision: {info['environment']['revision'][:12]}", fontsize=8)
    fig.tight_layout(rect=(0, .105, 1, 1))
    write(fig, out, "runtime_by_problem")


def totals(rows, info, out):
    fig, ax = plt.subplots(figsize=(10, 7))
    languages = info["languages"]
    valid_walls = []
    no_split = False
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
                    ha="center", transform=ax.get_xaxis_transform(), fontsize=9)
            continue
        wall = float(summary["suite_wall_ms"])
        child = sum(float(row["runtime_ms"]) for row in rows[language].values())
        startup = 0.0
        execution = 0.0
        split = True
        for row in rows[language].values():
            _, part_start, part_exec = parts(row)
            if part_start is None:
                split = False
                break
            startup += part_start
            execution += part_exec
        overhead = max(wall - child, 0.0)
        valid_walls.append(wall)
        if split:
            draw_timed_bar(ax, i, .62, child, startup, execution, language, extras=overhead)
            annotation = (f"{wall:.2f} [ms] total\n"
                          f"startup ~{startup:.2f} + exec ~{execution:.2f}\n"
                          f"overhead ~{overhead:.2f} [ms]\n{count}/{count} pass")
        else:
            no_split = True
            draw_timed_bar(ax, i, .62, wall, None, None, language)
            annotation = f"{wall:.2f} [ms] total\nno startup split\n{count}/{count} pass"
        ax.plot(i, child, "D", color="white", markeredgecolor="black", markersize=6, zorder=6)
        ax.annotate(annotation, (i, wall), xytext=(0, 7), textcoords="offset points",
                    ha="center", fontsize=8)
    ax.set_xticks(range(len(languages)), [l.upper() for l in languages])
    ax.set_ylabel("Sequential suite wall-clock duration [ms]")
    ax.set_title("Complete-suite startup, execution and runner overhead")
    ax.grid(axis="y", linestyle=":", alpha=.4)
    if valid_walls:
        ax.set_ylim(0, max(valid_walls) * 1.48)
    legend = legend_handles(languages, overhead=True, unattributed=no_split)
    legend.append(Line2D([], [], linestyle="none", marker="D", color="white",
                         markeredgecolor="black", label="End of summed child process times"))
    ax.legend(handles=legend, fontsize=8, ncol=2)
    fig.text(.04, .049, "Full-suite bar = startup estimate + execution residual + runner overhead. "
             "Diamond = end of child durations.", fontsize=9)
    fig.text(.04, .026, "Partial/failed suites have no total bar. Startup is estimated with no-op probes "
             "rather than measured within each solver.", fontsize=8)
    fig.text(.04, .008, "Algorithms, libraries and fixture loading differ; not an intrinsic language-speed ranking.",
             fontsize=8)
    fig.tight_layout(rect=(0, .095, 1, 1))
    write(fig, out, "totals")


def matching_problem_ids(rows, languages):
    """Exact intersection of implemented problem IDs, not a union of partial suites."""
    if len(languages) < 2:
        return []
    return sorted(set.intersection(*(set(rows[lang]) for lang in languages)))


def matched_subset_totals(rows, info, out):
    """Compare equal problem sets; never disguise partial language coverage as full."""
    languages = info["languages"]
    numbers = matching_problem_ids(rows, languages)
    if not numbers:
        return
    fig, ax = plt.subplots(figsize=(10, 7))
    valid_times = []
    no_split = False
    for index, language in enumerate(languages):
        passed = [n for n in numbers if rows[language][n]["status"] == "pass"]
        if len(passed) != len(numbers):
            ax.text(index, .05, f"{len(passed)}/{len(numbers)} passed\n"
                    "No comparable total", ha="center", transform=ax.get_xaxis_transform())
            continue
        durations = [parts(rows[language][n]) for n in numbers]
        duration = sum(p[0] for p in durations)
        valid_times.append(duration)
        split = all(p[1] is not None for p in durations)
        startup = sum(p[1] for p in durations) if split else None
        execution = sum(p[2] for p in durations) if split else None
        if not split:
            no_split = True
        draw_timed_bar(ax, index, .6, duration, startup, execution, language)
        label = (f"{duration:.2f} [ms] total\nstartup ~{startup:.2f} + exec ~{execution:.2f}"
                 if split else f"{duration:.2f} [ms] total\nno startup split")
        ax.annotate(label, (index, duration), xytext=(0, 8),
                    textcoords="offset points", ha="center", fontsize=9)
    if valid_times:
        # Log prevents much slower interpreters from making compiled bars invisible.
        ax.set_yscale("log")
        ax.set_ylim(min(valid_times) / 2.5, max(valid_times) * 3.5)
    ax.set_xticks(range(len(languages)), [lang.upper() for lang in languages])
    ax.set_ylabel("Sum of measured child wall times [ms] (log scale)")
    ax.set_title(f"Equal-coverage comparison: {len(numbers)} shared Euler problems\n"
                 "Estimated startup followed by execution residual")
    ax.grid(axis="y", linestyle=":", alpha=.4)
    ax.legend(handles=legend_handles(languages, unattributed=no_split), ncol=2,
              fontsize=8, loc="upper left")
    num_text = ", ".join(map(str, numbers))
    if len(num_text) > 125:
        num_text = num_text[:122] + "… (see metadata/CSV)"
    fig.text(.03, .050, "Shared Euler IDs: " + num_text, fontsize=8)
    fig.text(.03, .029, "Startup = same-runtime no-op median (not per-solver). Execution = remaining "
             "observed wall time, including imports/I/O.", fontsize=8)
    fig.text(.03, .009, "Compilation excluded. Missing/failed is not zero; no statistical error bars; "
             "different algorithms may dominate timing.", fontsize=8)
    fig.tight_layout(rect=(0, .11, 1, 1))
    write(fig, out, "matched_subset_totals")


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
    matched_subset_totals(rows, info, run / "plots")
    print(f"Saved SVG/PNG/PDF charts under {run / 'plots'}")


if __name__ == "__main__":
    main()
