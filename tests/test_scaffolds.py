"""Contracts separating incomplete scaffolds from implemented solutions."""
import importlib.util
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "scaffold_euler", ROOT / "scripts" / "scaffold_euler.py")
scaffold_euler = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(scaffold_euler)


class ScaffoldTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.catalog = json.loads(scaffold_euler.CATALOG.read_text(encoding="utf-8"))
        cls.expected = scaffold_euler.expected_scaffolds(cls.catalog)

    def test_snapshot_is_contiguous(self):
        maximum = self.catalog["maximum_problem"]
        self.assertEqual([p["number"] for p in self.catalog["problems"]],
                         list(range(1, maximum + 1)))

    def test_scaffold_ranges_and_counts(self):
        by_language = {language: [] for language in scaffold_euler.LANGUAGES}
        for path in self.expected:
            by_language[path.parent.name].append(path)
        self.assertEqual(len(by_language["c"]), self.catalog["maximum_problem"] - 255)
        self.assertEqual(len(by_language["rust"]), self.catalog["maximum_problem"] - 255)
        self.assertEqual(len(by_language["forth"]), self.catalog["maximum_problem"] - 120)
        factor_solutions = len(list((ROOT / "solutions/factor").glob("p*.factor")))
        self.assertEqual(len(by_language["factor"]),
                         self.catalog["maximum_problem"] - factor_solutions)

    def test_generated_tree_matches_catalog(self):
        self.assertEqual(scaffold_euler.generate(self.catalog, check=True), 0)

    def test_scaffolds_are_outside_solution_tree_and_fail_explicitly(self):
        for path, content in self.expected.items():
            self.assertNotIn(ROOT / "solutions", path.parents)
            self.assertIn("excluded from coverage", content)
            self.assertIn("not implemented", content)


if __name__ == "__main__":
    unittest.main()
