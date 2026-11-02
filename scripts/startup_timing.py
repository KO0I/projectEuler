#!/usr/bin/env python3
"""Reusable, explicitly *estimated* startup/compute split for Euler profiles.

A no-op child with the same language/runtime invocation measures process and
minimal interpreter startup. It cannot pinpoint a particular solver's first
algorithm instruction. The remaining observed wall time also includes the
solver's module loading, I/O, shutdown, and parent-side capture overhead.
"""
from __future__ import annotations

import math
import os
from pathlib import Path
import shutil
import statistics
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]


def isolated_factor_environment(*, root: Path = ROOT) -> dict[str, str]:
    """Keep Factor startup state inside the build tree.

    Factor 0.102 only processes ``-roots`` when user initialization is enabled.
    Pointing its home at an empty generated directory permits vocabulary roots
    while excluding the caller's ``.factor-rc`` and ``.factor-roots`` files.
    """
    factor_home = root / "build" / "factor-home"
    factor_home.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ)
    env["HOME"] = str(factor_home)
    env["USERPROFILE"] = str(factor_home)
    return env


def probe_command(language: str, *, root: Path = ROOT, factor_vm: str | None = None) -> list[str]:
    """No-op command matching the solver's runtime and invocation style."""
    ext = ".exe" if os.name == "nt" else ""
    if language == "c":
        return [str(root / "build" / "bench" / ("noop-c" + ext))]
    if language == "rust":
        return [str(root / "build" / "bench" / ("noop-rust" + ext))]
    if language == "forth":
        return ["gforth", str(root / "support" / "bench" / "noop.fs")]
    if language == "factor":
        vm = factor_vm or os.environ.get("FACTOR_VM") or str(root / ".toolchains/factor/factor")
        return [vm, f"-roots={root / 'solutions' / 'factor'}",
                str(root / "support" / "bench" / "noop.factor")]
    raise ValueError(f"Unknown language: {language}")


def calibrate(language: str, *, samples: int = 5, timeout: float = 30.0,
              root: Path = ROOT, factor_vm: str | None = None) -> dict:
    """Record individual no-op observations; median is a baseline, not exact startup.

    Probe compilation, if needed, occurs through Make/PowerShell *before* this
    function. The solver is never invoked to determine its own startup.
    """
    if not 1 <= samples <= 99:
        raise ValueError("Startup samples must be in 1..99")
    cmd = probe_command(language, root=root, factor_vm=factor_vm)
    result = {
        "status": "unavailable", "method": "median-of-noop-processes",
        "baseline_ms": None, "sample_ms": [], "samples_requested": samples,
        "command": cmd, "message": "",
        "meaning": "Minimal same-runtime process baseline; not an exact solver startup measurement",
    }
    if not shutil.which(cmd[0]) and not Path(cmd[0]).is_file():
        result["message"] = "No-op executable or runtime is not installed/built"
        return result
    for _ in range(samples):
        start = time.perf_counter_ns()
        try:
            kwargs = ({"env": isolated_factor_environment(root=root)}
                      if language == "factor" else {})
            proc = subprocess.run(cmd, cwd=root, capture_output=True,
                                  text=True, timeout=timeout, check=False, **kwargs)
        except (OSError, subprocess.TimeoutExpired) as exc:
            result["status"] = "failed"
            result["message"] = f"No-op calibration failed: {exc}"
            return result
        ms = (time.perf_counter_ns() - start) / 1_000_000
        if proc.returncode != 0 or proc.stdout.strip():
            result["status"] = "failed"
            result["message"] = ("No-op returned nonzero or wrote unexpected stdout: " +
                                 proc.stderr.strip()[:200])
            return result
        result["sample_ms"].append(round(ms, 6))
    result["baseline_ms"] = round(statistics.median(result["sample_ms"]), 6)
    result["status"] = "measured"
    return result


def split_wall_time(wall_ms: str | float, calibration: dict | None) -> tuple[str, str, str]:
    """Split total into capped startup baseline and residual, never fabricate data.

    Exact six-decimal arithmetic consistency is kept for CSV/plot comparisons.
    When the baseline exceeds the solution time, note the censoring explicitly.
    """
    if not calibration or calibration.get("status") != "measured":
        return "", "", "unavailable"
    total = round(float(wall_ms), 6)
    baseline = calibration.get("baseline_ms")
    if not math.isfinite(total) or total <= 0 or not isinstance(baseline, (int, float)) \
            or not math.isfinite(baseline) or baseline <= 0:
        return "", "", "unavailable"
    startup = min(total, round(float(baseline), 6))
    execution = round(total - startup, 6)
    return f"{startup:.6f}", f"{execution:.6f}", ("capped" if baseline > total else "estimated")
