"""Probes measure no-op children, never rerun the Euler solutions."""
import importlib.util
from pathlib import Path
import subprocess
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location("startup_timing", ROOT / "scripts/startup_timing.py")
timing = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(timing)


class StartupTimingTests(unittest.TestCase):
    def test_probe_commands_use_same_runtime_and_factor_root(self):
        self.assertEqual(timing.probe_command("c", root=ROOT)[0],
                         str(ROOT / "build/bench/noop-c"))
        self.assertEqual(timing.probe_command("rust", root=ROOT)[0],
                         str(ROOT / "build/bench/noop-rust"))
        self.assertEqual(timing.probe_command("forth", root=ROOT)[0], "gforth")
        f = timing.probe_command("factor", root=ROOT, factor_vm="/vm/factor")
        self.assertEqual(f[0], "/vm/factor")
        self.assertIn(f"-roots={ROOT / 'solutions/factor'}", f)
        self.assertNotIn("-no-user-init", f)

    def test_median_of_independent_noop_processes(self):
        run = subprocess.CompletedProcess(["noop"], 0, "", "")
        with patch.object(timing.shutil, "which", return_value="noop"), \
             patch.object(timing.subprocess, "run", return_value=run) as invoked, \
             patch.object(timing.time, "perf_counter_ns", side_effect=[
                 0, 1_000_000, 2_000_000, 5_000_000, 6_000_000, 8_000_000
             ]):
            result = timing.calibrate("c", samples=3)
        self.assertEqual(invoked.call_count, 3)
        self.assertEqual(result["sample_ms"], [1., 3., 2.])
        self.assertEqual(result["baseline_ms"], 2.)
        self.assertEqual(result["status"], "measured")

    def test_missing_probe_is_not_zero_or_estimated(self):
        with patch.object(timing.shutil, "which", return_value=None), \
             patch.object(timing.Path, "is_file", return_value=False), \
             patch.object(timing.subprocess, "run") as run:
            result = timing.calibrate("c")
        run.assert_not_called()
        self.assertEqual(result["status"], "unavailable")
        self.assertIsNone(result["baseline_ms"])
        self.assertEqual(timing.split_wall_time("9.4", result), ("", "", "unavailable"))

    def test_failed_probe_prevents_split(self):
        run = subprocess.CompletedProcess(["noop"], 1, "", "bad")
        with patch.object(timing.shutil, "which", return_value="noop"), \
             patch.object(timing.subprocess, "run", return_value=run):
            result = timing.calibrate("c", samples=1)
        self.assertEqual(result["status"], "failed")
        self.assertEqual(timing.split_wall_time("5", result), ("", "", "unavailable"))

    def test_additive_estimate(self):
        cal = {"status": "measured", "baseline_ms": 1.123456}
        self.assertEqual(timing.split_wall_time("5.654321", cal),
                         ("1.123456", "4.530865", "estimated"))

    def test_clipping_is_explicit_not_negative_execution(self):
        cal = {"status": "measured", "baseline_ms": 9.0}
        self.assertEqual(timing.split_wall_time("0.765432", cal),
                         ("0.765432", "0.000000", "capped"))

    def test_bad_sample_count_rejected(self):
        for samples in (0, 100, -1):
            with self.assertRaises(ValueError):
                timing.calibrate("c", samples=samples)


if __name__ == "__main__":
    unittest.main()
