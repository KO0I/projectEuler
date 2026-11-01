"""Matched subset profiling uses the intersection, never padded zeros."""
import importlib.util
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[1]
SPEC=importlib.util.spec_from_file_location("plots", ROOT/"scripts/plots.py")
plots=importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(plots)

class MatchCoverageTests(unittest.TestCase):
    def test_intersection_only(self):
        rows={
            "c": {1:{},2:{},3:{}},
            "rust": {1:{},2:{},3:{}},
            "factor": {1:{},3:{}},
        }
        self.assertEqual(plots.matching_problem_ids(rows,["c","rust","factor"]),[1,3])
    def test_dedicated_language_not_group_comparison(self):
        self.assertEqual(plots.matching_problem_ids({"factor": {1:{}}},["factor"]),[])
    def test_empty_shared_set(self):
        self.assertEqual(plots.matching_problem_ids({"c":{1:{}}, "factor":{2:{}}},
                                                   ["c","factor"]),[])
if __name__=="__main__":unittest.main()
