"""Factor metadata and VM selection must be deliberate; no GNU coreutils collision."""
import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location(
    "bench", Path(__file__).resolve().parents[1] / "scripts/bench.py"
)
bench = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(bench)

class FactorBenchTests(unittest.TestCase):
    def test_partial_factor_coverage(self):
        manifest = [{"number": n} for n in range(1, 120)]
        selection = bench.selected_problems("factor", manifest)
        self.assertEqual([p["number"] for p in selection],
                         sorted(bench.FACTOR_PROBLEMS))
        self.assertEqual(len(selection), 12)
        self.assertEqual(bench.selected_problems("c", manifest), manifest)

    def test_factor_does_not_use_coreutils(self):
        with patch.dict(bench.os.environ, {}, clear=True):
            vm = bench.factor_vm()
            self.assertNotEqual(vm, "factor")
            self.assertTrue(vm.endswith(".toolchains/factor/factor"))

    def test_custom_factor_vm_and_one_process_command(self):
        with patch.dict(bench.os.environ, {"FACTOR_VM": "/opt/Factor/factor"}):
            cmd = bench.command("factor", 9, "ignored")
            self.assertEqual(cmd, ["/opt/Factor/factor", "-no-user-init",
                                   str(bench.ROOT / "solutions/factor/p9.factor")])

    def test_unknown_factor_coverage_rejected(self):
        with self.assertRaises(ValueError):
            bench.selected_problems("factor", [{"number": 1}])

if __name__ == "__main__":
    unittest.main()
