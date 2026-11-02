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

class StartupPlotTests(unittest.TestCase):
    def row(self, number, language, elapsed, startup=None):
        result = dict(run_id='sample', language=language, problem=str(number),
                      runtime_ms=f'{elapsed:.6f}', status='pass')
        if startup is not None:
            result.update(startup_estimate_ms=f'{startup:.6f}',
                          execution_residual_ms=f'{elapsed-startup:.6f}',
                          split_status='estimated')
        return result

    def test_legacy_rows_are_not_invented(self):
        self.assertEqual(plots.parts(self.row(1, 'c', 2)), (2, None, None))

    def test_bad_additive_split_is_rejected(self):
        row = self.row(1, 'c', 5, 1)
        row['execution_residual_ms'] = '100.000000'
        with self.assertRaisesRegex(ValueError, 'must match'):
            plots.parts(row)

    def test_result_summary_keeps_timeouts_separate(self):
        records = ([{'status': 'pass'}] * 115
                   + [{'status': 'timeout'}] * 5)
        self.assertEqual(plots.status_summary(records), '115 pass, 5 timeout')

    def test_problem_ranges_are_readable_and_complete(self):
        groups = plots.problem_ranges(range(1, 256))
        self.assertEqual(len(groups), 26)
        self.assertEqual([(group[0], group[-1]) for group in groups[:3]],
                         [(1, 10), (11, 20), (21, 30)])
        self.assertEqual((groups[-1][0], groups[-1][-1]), (251, 255))
        self.assertTrue(all(len(group) == 10 for group in groups[:-1]))
        self.assertEqual([number for group in groups for number in group],
                         list(range(1, 256)))

    def test_range_legend_does_not_report_status_from_another_figure(self):
        rows = {'forth': {
            33: {'status': 'timeout'},
            201: {'status': 'pass'},
        }}
        handles = plots.result_marker_handles(rows, ['forth'], [201])
        self.assertNotIn('Timed out', [handle.get_label() for handle in handles])

    def test_breakdown_bar_has_two_colors_hatches_and_boundary_line(self):
        import matplotlib.pyplot as plt
        fig, ax = plt.subplots()
        plots.draw_timed_bar(ax, 0, .6, 4.0, 1.25, 2.75, 'c')
        # Each logical segment has a fill, a white-hatch overlay, and a dark
        # outline.  The layers decouple hatch color from border color.
        self.assertEqual(len(ax.patches), 6)
        self.assertAlmostEqual(ax.patches[0].get_height(), 1.25)
        self.assertAlmostEqual(ax.patches[3].get_height(), 2.75)
        self.assertAlmostEqual(ax.patches[3].get_y(), 1.25)
        self.assertNotEqual(ax.patches[0].get_facecolor(), ax.patches[3].get_facecolor())
        self.assertEqual(ax.patches[1].get_edgecolor(), (1.0, 1.0, 1.0, 1.0))
        self.assertEqual(ax.patches[4].get_edgecolor(), (1.0, 1.0, 1.0, 1.0))
        self.assertNotEqual(ax.patches[1].get_hatch(), ax.patches[4].get_hatch())
        self.assertTrue(ax.collections, 'Startup/compute boundary must be a visible line')
        plt.close(fig)

    def test_per_problem_panel_omits_startup_estimate(self):
        import matplotlib.pyplot as plt
        from unittest.mock import patch
        info = {'run_id': 'sample', 'languages': ['c'],
                'environment': {'revision': 'unavailable'}}
        rows = {'c': {1: self.row(1, 'c', 5, 1)}}
        with patch.object(plots, 'write') as write:
            plots.runtime_by_problem_panel(
                rows, info, Path('.'), [1], (0, 10), 'sample')
        fig = write.call_args.args[0]
        self.assertEqual(len(fig.axes[0].patches), 3)
        self.assertAlmostEqual(fig.axes[0].patches[0].get_height(), 4.0)
        plt.close(fig)

    def test_zero_residual_is_an_explicit_diamond_not_an_absent_bar(self):
        import matplotlib.pyplot as plt
        from unittest.mock import patch
        info = {'run_id': 'sample', 'languages': ['c'],
                'environment': {'revision': 'unavailable'}}
        rows = {'c': {1: self.row(1, 'c', 1, 1)}}
        with patch.object(plots, 'write') as write:
            plots.runtime_by_problem_panel(
                rows, info, Path('.'), [1], (0, 10), 'sample')
        fig = write.call_args.args[0]
        ax = fig.axes[0]
        self.assertIn('D', [line.get_marker() for line in ax.lines])
        labels = [text.get_text() for text in ax.get_legend().get_texts()]
        self.assertIn('0 ms residual (at/below startup estimate)', labels)
        plt.close(fig)

    def test_chart_rendered_for_single_language_without_fake_numbers(self):
        import tempfile
        from unittest.mock import patch
        with tempfile.TemporaryDirectory() as d:
            info = {'schema': 3, 'run_id': 'sample', 'languages': ['c'],
                    'problems': [1,2], 'environment': {'revision':'unavailable'},
                    'startup_calibrations': {'c': {'status':'measured','baseline_ms':1.0}},
                    'language_summaries': {'c': {
                        'passed':2, 'complete':True,'full_coverage':True,
                        'suite_wall_ms':9.0, 'passing_child_runtime_ms':8.0}}}
            data = {1:self.row(1,'c',3,1), 2:self.row(2,'c',5,1)}
            rows = {'c':data}
            stems = plots.runtime_by_problem(rows, info, Path(d))
            plots.totals(rows, info, Path(d))
            self.assertEqual(stems, ['runtime_by_problem_001_002'])
            self.assertTrue((Path(d)/'runtime_by_problem_001_002.svg').is_file())
            self.assertTrue((Path(d)/'totals.pdf').is_file())

    def test_matched_subset_plot_has_both_components(self):
        import tempfile
        with tempfile.TemporaryDirectory() as d:
            info = {'run_id':'sample','languages':['c','rust'], 'problems':[1],
                    'environment': {'revision':'unavailable'}}
            rows = {'c':{1:self.row(1,'c',2,1)},
                    'rust':{1:self.row(1,'rust',4,1.5)}}
            plots.matched_subset_totals(rows,info,Path(d))
            self.assertTrue((Path(d)/'matched_subset_totals.png').is_file())
