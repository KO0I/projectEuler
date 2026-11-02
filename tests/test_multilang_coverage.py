"""Coverage and invocation tests; these do not execute the solvers."""
import importlib.util
from pathlib import Path
import unittest
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def module(name, file):
    spec = importlib.util.spec_from_file_location(name, ROOT / file)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


class LanguageCoverage(unittest.TestCase):
    @unittest.skipUnless(shutil.which('gforth'), 'Gforth is not installed')
    def test_forth_problem_loads_without_other_solvers(self):
        """Exercise the real interpreter, including each problem's private dependencies."""
        for number in range(1, 121):
            other = 2 if number == 1 else 1
            with self.subTest(problem=number):
                code = (f'true constant euler-testing '
                        f's" solutions/forth/p{number}.fs" included '
                        f's" p{number}" find-name 0= -1 and throw '
                        f's" p{other}" find-name 0<> -1 and throw bye')
                result = subprocess.run(['gforth', '-e', code], cwd=ROOT,
                                        capture_output=True, text=True, timeout=60)
                self.assertEqual(result.returncode, 0, result.stderr)

    def test_native_entrypoints_and_helpers(self):
        checker = module('coverage', 'scripts/check_language_coverage.py')
        self.assertEqual(checker.audit(), {'rust': 255, 'forth': 120, 'factor': 191})

    def test_benchmark_selected_problems(self):
        import json
        bench = module('bench_full', 'scripts/bench.py')
        problems = json.loads((ROOT / 'tests/answers.json').read_text())['problems']
        expected = {'c': 255, 'rust': 255, 'forth': 120, 'factor': 191}
        for lang in ('c', 'rust', 'forth', 'factor'):
            with self.subTest(lang=lang):
                self.assertEqual(len(bench.selected_problems(lang, problems)), expected[lang])

    def test_factor_vocab_root(self):
        bench = module('bench_factor', 'scripts/bench.py')
        command = bench.command('factor', 120, 'square_remainders_120')
        self.assertIn(f"-roots={ROOT / 'solutions/factor'}", command)
        self.assertNotEqual(command[0], '/usr/bin/factor')


if __name__ == '__main__':
    unittest.main()
