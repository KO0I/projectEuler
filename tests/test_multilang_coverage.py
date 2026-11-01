"""Coverage and invocation tests; these do not execute the solvers."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


def module(name, file):
    spec = importlib.util.spec_from_file_location(name, ROOT / file)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


class LanguageCoverage(unittest.TestCase):
    def test_native_entrypoints_and_helpers(self):
        checker = module('coverage', 'scripts/check_language_coverage.py')
        self.assertEqual(checker.audit(), {'rust': 120, 'forth': 120, 'factor': 120})

    def test_benchmark_selected_problems(self):
        import json
        bench = module('bench_full', 'scripts/bench.py')
        problems = json.loads((ROOT / 'tests/answers.json').read_text())['problems']
        for lang in ('c', 'rust', 'forth', 'factor'):
            with self.subTest(lang=lang):
                self.assertEqual(len(bench.selected_problems(lang, problems)), 120)

    def test_factor_vocab_root(self):
        bench = module('bench_factor', 'scripts/bench.py')
        command = bench.command('factor', 120, 'square_remainders_120')
        self.assertIn(f"-roots={ROOT / 'solutions/factor'}", command)
        self.assertNotEqual(command[0], '/usr/bin/factor')


if __name__ == '__main__':
    unittest.main()
