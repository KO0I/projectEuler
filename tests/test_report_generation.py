"""Contracts for generated report reference material and Pascal-pyramid plots."""
import importlib.util
import json
import math
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


def load_script(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / "scripts" / f"{name}.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


report = load_script("generate_report_appendices")
plots = load_script("math_plots")


class ReportGenerationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.problems = json.loads(report.ANSWERS.read_text())["problems"]

    def test_walkthrough_covers_every_manifest_problem(self):
        generated = report.generate_walkthroughs(self.problems)
        self.assertEqual(generated.count(r"\subsection{Problem "), 255)
        self.assertIn("Problem 154: Exploring Pascal's Pyramid", generated)
        self.assertIn("solutions/rust/src/bin/p255.rs", generated)

    def test_api_contains_symbols_but_not_c_control_flow(self):
        generated = report.generate_api()
        self.assertIn(r"\detokenize{euler_is_prime}", generated)
        self.assertIn(r"\detokenize{modular-pow}", generated)
        self.assertNotIn(r"\detokenize{if}", generated)
        self.assertNotIn(r"\detokenize{while}", generated)

    def test_factor_walkthrough_uses_algorithm_vocabulary(self):
        problem = self.problems[83]
        path, text = report.source_material(problem, "factor")
        self.assertEqual(path.name, "advanced.factor")
        self.assertIn(":: p84", report.snippet(text, "factor", 84))

    def test_trinomial_layer_sums_to_three_to_n(self):
        for n in range(12):
            total = sum(
                plots.trinomial(n, i, j)
                for i in range(n + 1)
                for j in range(n - i + 1)
            )
            self.assertEqual(total, 3**n)

    def test_pyramid_has_tetrahedral_point_count(self):
        for maximum in (0, 1, 5, 14):
            points = plots.pascal_pyramid_coefficients(maximum)
            self.assertEqual(len(points), math.comb(maximum + 3, 3))


if __name__ == "__main__":
    unittest.main()
