"""Coverage metadata follows native entrypoints in all four languages."""
import importlib.util
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location("bench", ROOT / "scripts/bench.py")
bench = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(bench)


class CoverageTests(unittest.TestCase):
    def test_full_multilang_source_coverage(self):
        problems = json.loads((ROOT / "tests/answers.json").read_text())["problems"]
        self.assertEqual({p["number"] for p in bench.selected_problems("c", problems)},
                         set(range(1, 121)))
        for language in ("rust", "forth", "factor"):
            self.assertEqual({p["number"] for p in bench.selected_problems(language, problems)},
                             set(range(1, 121)))

    def test_c_only_entry_is_excluded_from_rust_and_forth(self):
        problems = [{"number": 120, "languages": ["c"]}]
        self.assertEqual(bench.selected_problems("c", problems), problems)
        self.assertEqual(bench.selected_problems("rust", problems), [])
        self.assertEqual(bench.selected_problems("forth", problems), [])


if __name__ == "__main__":
    unittest.main()
