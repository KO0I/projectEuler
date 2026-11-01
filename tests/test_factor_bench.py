"""Factor metadata and VM selection must be deliberate; no GNU coreutils collision."""
import importlib.util
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location(
    "bench", Path(__file__).resolve().parents[1] / "scripts/bench.py"
)
bench = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(bench)

class FactorBenchTests(unittest.TestCase):
    def test_manifest_factor_coverage(self):
        manifest = [{"number": n, "languages": ["c", "factor"]} for n in range(1, 121)]
        selection = bench.selected_problems("factor", manifest)
        self.assertEqual([p["number"] for p in selection], list(range(1, 121)))
        self.assertEqual(bench.selected_problems("c", manifest), manifest)

    def test_factor_does_not_use_coreutils(self):
        with patch.dict(bench.os.environ, {}, clear=True):
            vm = bench.factor_vm()
            self.assertNotEqual(vm, "factor")
            self.assertTrue(vm.endswith(".toolchains/factor/factor"))

    def test_custom_factor_vm_and_one_process_command(self):
        with patch.dict(bench.os.environ, {"FACTOR_VM": "/opt/Factor/factor"}):
            cmd = bench.command("factor", 9, "ignored")
            self.assertEqual(cmd, ["/opt/Factor/factor",
                                   f"-roots={bench.ROOT / 'solutions/factor'}",
                                   str(bench.ROOT / "solutions/factor/p9.factor")])

    def test_factor_home_is_generated_and_isolated(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            with patch.dict(bench.os.environ, {"HOME": "/real/home", "KEPT": "yes"}, clear=True):
                env = bench.isolated_factor_environment(root=root)
            expected = root / "build/factor-home"
            self.assertEqual(env["HOME"], str(expected))
            self.assertEqual(env["USERPROFILE"], str(expected))
            self.assertEqual(env["KEPT"], "yes")
            self.assertTrue(expected.is_dir())

    def test_non_factor_manifest_entry_is_excluded(self):
        self.assertEqual(bench.selected_problems("factor", [{"number": 1}]), [])

if __name__ == "__main__":
    unittest.main()
