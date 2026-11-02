# Project Euler visualization index

Generate with `make plots-math`. All 26 figures (five pre-existing, 18 Euler figures,
and three Pascal's-pyramid figures)
are saved to `build/math_plots/` as **SVG, PDF, and PNG**. The linked LaTeX
report includes the relevant figures automatically when plots have been generated
before `make docs`. No solver is profiled while creating these figures.

## Legend and provenance

- **Actual input/results:** Problem 11 draws the committed 20x20 grid and checks
  the maximum product equals 70,600,674; problem 28 sums the actual 1001x1001
  spiral corner values; problem 39 counts perimeters through the required 1000.
- **Exact mathematics, sampled bounds:** Pythagorean counts, lattice binomial
  coefficients, prime ratios, rectangle counts, and tiling recurrences show their
  specified finite domains.
- **Illustrations, NOT complete Euler solvers:** Reduced example triangles,
  matrices, grids, darts, Sudoku, and networks explicitly say "example" or
  "sample". No solution to an unimplemented problem is claimed.
- **Accessibility:** Figures prefer grayscale and distinguish their data by
  hatching, line style, symbol, or direct annotation; red marks are an optional
  emphasis layer, never the sole encoded distinction.
- **Units:** Geometric axes explicitly label cells or units. Benchmark
  time axes label [ms], with hollow circle = not implemented and X = failure.
- **Timing discipline:** No timing data is generated or inferred by this script.

| Euler # | Problem | Figure file stem | Scope |
| ---: | --- | --- | --- |
| 9 | Special Pythagorean Triplet | `pythagorean_perimeters` | Euclid triples; p=1000 |
| 11 | Largest Product in a Grid | `grid_largest_product` | Actual repo grid |
| 15 | Lattice Paths | `lattice_paths` | 6x6 example |
| 18 | Maximum Path Sum I | `maximum_path_sum_demo` | Four-row example |
| 28 | Number Spiral Diagonals | `spiral_diagonal_sum`, `ulam_spiral` | Full diagonal sum; sample Ulam |
| 39 | Integer Right Triangles | `pythagorean_perimeters` | Perimeters to 1000 |
| 58 | Spiral Primes | `spiral_prime_ratio` | Sample side ≤2501 |
| 67 | Maximum Path Sum II | `maximum_path_sum_demo` | Four-row DP example |
| 68 | Magic 5-gon Ring | `magic_5gon` | Valid example, common sum 14 |
| 75 | Singular Integer Right Triangles | `pythagorean_perimeters` | Count to p=1500 |
| 81 | Path Sum: Two Ways | `path_sum_directions` | 5x5 example matrix |
| 82 | Path Sum: Three Ways | `path_sum_directions` | 5x5 example matrix |
| 83 | Path Sum: Four Ways | `path_sum_directions` | 5x5 example matrix |
| 84 | Monopoly Odds | `monopoly_dice_kernel` | 2d4 kernel ONLY, no cards/jail |
| 85 | Counting Rectangles | `counting_rectangles` | Counts, 2M contour |
| 86 | Cuboid Route | `cuboid_routes` | Fixed longest edge 25 |
| 90 | Cube Digit Pairs | `cube_digit_pairs` | Valid example, 6/9 equivalence |
| 91 | Right Triangles with Integer Coordinates | `coordinate_right_triangles` | 5x5 example |
| 94 | Almost Equilateral Triangles | `almost_equilateral` | 5-5-6 example |
| 96 | Su Doku | `sudoku_demo` | Solved 9x9 example |
| 102 | Triangle Containment | `triangle_containment` | Two example triangles |
| 107 | Minimal Network | `minimal_network` | Synthetic weighted graph |
| 109 | Darts | `darts_checkout_counts` | Distinct checkouts below 80 |
| 114 | Counting Block Combinations I | `block_tilings` | Recurrence, lengths ≤20 |
| 115 | Counting Block Combinations II | `block_tilings` | Scaled min red run =4 |
| 116 | Red, Green or Blue Tiles | `block_tilings` | One color per row |
| 117 | Red, Green, and Blue Tiles | `block_tilings` | Mixed lengths 2/3/4 |
| 154 | Exploring Pascal's Pyramid | `pascal_pyramid_tetrahedron` | Exact coefficients through layer 14 |
| 154 | Exploring Pascal's Pyramid | `pascal_pyramid_layer_surface` | Exact layer 24, log-height surface |
| 154 | Exploring Pascal's Pyramid | `pascal_pyramid_parity` | Odd coefficients through layer 31 |

These are official Project Euler IDs, **not** the line numbers of the user's
original list. Problem 10 (Summation of Primes) has an existing extra figure,
`prime_distribution`, and problems 14 and 27 retain prior figures.

Full-size problem input fixtures for 67, 81, 82, 83, 96, 102 and 107 have not
been added to this repository by this change. The visualizations do not pretend
otherwise.
