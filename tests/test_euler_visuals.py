"""Deterministic arithmetic checks for new Euler teaching visualizations."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location("euler_visuals",
                                               ROOT / "scripts" / "euler_visuals.py")
vis = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(vis)

class VisualizationMath(unittest.TestCase):
    def test_minimal_path_sum_samples(self):
        m = vis.MATRIX
        self.assertEqual(vis.shortest_grid(m, [(0,0)], [(4,4)],
                        ((0,1),(1,0)))[0], 2427)
        self.assertEqual(vis.shortest_grid(m, [(r,0) for r in range(5)],
                        [(r,4) for r in range(5)],
                        ((0,1),(-1,0),(1,0)))[0], 994)
        self.assertEqual(vis.shortest_grid(m, [(0,0)], [(4,4)],
                        ((0,1),(0,-1),(1,0),(-1,0)))[0], 2297)

    def test_origin_containment_orientation(self):
        tri = vis.np.array([[-3.,-2.],[4.,-1.],[1.,4.]])
        out = vis.np.array([[1.,2.],[4.,1.],[2.,5.]])
        self.assertTrue(vis.origin_in_triangle(tri))
        self.assertTrue(vis.origin_in_triangle(tri[::-1]))
        self.assertFalse(vis.origin_in_triangle(out))

    def test_singular_count_triples_small_sample(self):
        from math import gcd, isqrt
        counts = [0] * 121
        for m in range(2, 9):
            for n in range(1, m):
                perimeter = 2*m*(m+n)
                if perimeter > 120 or (m-n)%2==0 or gcd(m,n)!=1:
                    continue
                for p in range(perimeter,121,perimeter):
                    counts[p]+=1
        self.assertEqual(counts[12],1)
        self.assertEqual(counts[120],3)

    def test_red_separated_base_case(self):
        self.assertEqual(vis.red_separated(3,3), 2)
        self.assertEqual(vis.red_separated(4,3), 4)

if __name__ == "__main__":
    unittest.main()
