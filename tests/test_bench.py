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
    def test_wrong_answer(self):
        self.assertEqual(self.run_fake("123\n")["status"], "wrong-answer")
    def test_non_numeric_output(self):
        self.assertEqual(self.run_fake("answer: 233168\n")["status"], "invalid-output")
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
    def test_missing_is_unmeasured(self):
        with patch.object(bench, "command", return_value=["/not-present/euler-solver"]), \
             patch.object(bench.shutil, "which", return_value=None), \
             patch.object(bench.subprocess, "run") as invoke:
            row = bench.run_one("c", PROBLEM, 30, "test-run")
            invoke.assert_not_called()
            self.assertEqual(row["status"], "missing")
            self.assertEqual(row["runtime_ms"], "")

if __name__ == "__main__":
    unittest.main()
