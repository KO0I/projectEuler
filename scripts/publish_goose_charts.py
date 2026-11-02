#!/usr/bin/env python3
"""Publish execution-time bars from one saved Goose profile, without rerunning solvers."""
import argparse
from pathlib import Path
import shutil
import tempfile
import matplotlib
import plots


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run', type=Path, required=True, help='Saved profile directory containing metadata.json and CSVs')
    args = parser.parse_args()
    run, info, rows = plots.load(args.run)
    if 'goose' not in info['languages']:
        parser.error('The selected run has no Goose measurements')
    destination = run / 'charts'
    destination.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory() as directory:
        temporary = Path(directory)
        # Keep SVG labels as text so the published figures remain compact and editable.
        with matplotlib.rc_context({'svg.fonttype': 'none', 'svg.hashsalt': info['run_id']}):
            plots.runtime_by_problem(rows, info, temporary)
            plots.matched_subset_totals(rows, info, temporary)
        for source in temporary.glob('*.svg'):
            shutil.copy2(source, destination / source.name)
    print(f'Published {len(list(destination.glob("*.svg")))} figures from run {info["run_id"]}')


if __name__ == '__main__':
    main()
