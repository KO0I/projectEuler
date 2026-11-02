"""Harness tests use mocks, not real solver executions or warm-up runs."""
import importlib.util
from pathlib import Path
import subprocess
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location("bench", Path(__file__).resolve().parents[1] / "scripts/bench.py")
bench = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(bench)
PROBLEM = {"number": 1, "stem": "multiples_of_3_or_5_1", "answer": "233168"}

class BenchmarkTests(unittest.TestCase):
    def run_fake(self, output="233168\n", code=0):
        completed = subprocess.CompletedProcess(["solver"], code, stdout=output, stderr="")
        with patch.object(bench, "command", return_value=["solver"]), \
             patch.object(bench.shutil, "which", return_value="solver"), \
             patch.object(bench.subprocess, "run", return_value=completed) as invoke:
            row = bench.run_one("c", PROBLEM, 30, "test-run")
            invoke.assert_called_once()
            return row
    def test_single_invocation(self):
        self.assertEqual(self.run_fake()["status"], "pass")
    def test_single_solver_invocation_with_startup_split(self):
        calibration = {"status": "measured", "baseline_ms": 0.123456}
        completed = subprocess.CompletedProcess(["solver"], 0, stdout="233168\n", stderr="")
        with patch.object(bench, "command", return_value=["solver"]), \
             patch.object(bench.shutil, "which", return_value="solver"), \
             patch.object(bench.subprocess, "run", return_value=completed) as invoke, \
             patch.object(bench.time, "perf_counter_ns", side_effect=[0, 2_000_000]):
            row = bench.run_one("c", PROBLEM, 30, "test-run", calibration)
            invoke.assert_called_once()
        self.assertEqual(row["startup_estimate_ms"], "0.123456")
        self.assertEqual(row["execution_residual_ms"], "1.876544")
        self.assertEqual(row["runtime_ms"], "2.000000")
        self.assertEqual(row["split_status"], "estimated")
    def test_wrong_answer(self):
        self.assertEqual(self.run_fake("123\n")["status"], "wrong-answer")
    def test_noncanonical_output(self):
        self.assertEqual(self.run_fake("233168\nextra\n")["status"], "invalid-output")
        self.assertEqual(self.run_fake("233168")["status"], "invalid-output")
    def test_exact_non_integer_answer_formats(self):
        for answer in ("0.464399", "3D58725572C62302", "1,13717420,8", "123/59"):
            problem = {**PROBLEM, "answer": answer}
            completed = subprocess.CompletedProcess(["solver"], 0, stdout=answer + "\n", stderr="")
            with patch.object(bench, "command", return_value=["solver"]), \
                 patch.object(bench.shutil, "which", return_value="solver"), \
                 patch.object(bench.subprocess, "run", return_value=completed):
                self.assertEqual(bench.run_one("c", problem, 30, "test-run")["status"], "pass")
    def test_exit_failure(self):
        self.assertEqual(self.run_fake(code=1)["status"], "error")
    def test_timeout_is_not_zero(self):
        with patch.object(bench, "command", return_value=["solver"]), \
             patch.object(bench.shutil, "which", return_value="solver"), \
             patch.object(bench.subprocess, "run", side_effect=subprocess.TimeoutExpired("solver", 30)) as invoke:
            row = bench.run_one("c", PROBLEM, 30, "test-run")
            invoke.assert_called_once()
            self.assertEqual(row["status"], "timeout")
            self.assertNotEqual(row["runtime_ms"], "")
            self.assertEqual(row["startup_estimate_ms"], "")
    def test_missing_is_unmeasured(self):
        with patch.object(bench, "command", return_value=["/not-present/euler-solver"]), \
             patch.object(bench.shutil, "which", return_value=None), \
             patch.object(bench.subprocess, "run") as invoke:
            row = bench.run_one("c", PROBLEM, 30, "test-run")
            invoke.assert_not_called()
            self.assertEqual(row["status"], "missing")
            self.assertEqual(row["runtime_ms"], "")
            self.assertEqual(row["execution_residual_ms"], "")

if __name__ == "__main__":
    unittest.main()
