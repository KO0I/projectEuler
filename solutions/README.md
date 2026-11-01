# Project Euler solution index

C covers every official problem number from 1 through 120. The 37 solutions already
addressed in the report are retained; the other 83 have new C implementations.

Each new C file contains its algorithm. The older C entry points retain their
implementations in `support/c/euler_common.c`. Historical attempts are in
`c/variants/`; they are not the programs built by the default target.

Rust, Gforth, and Factor also have entry points for every listed problem.
The implementation column records source coverage, not validation status.

See [new C methods and bounds](../docs/C_1_120.md),
[C validation](../docs/VALIDATION_C_1_120.md),
[Gforth validation](../docs/VALIDATION_FORTH_1_120.md), and
[the existing report](../explanations/latex/project_euler_c_explanations.tex).

| Number | Problem | C source | Status | Other implementations |
| ---: | --- | --- | --- | --- |
| 1 | [Multiples of 3 or 5](https://projecteuler.net/problem=1) | [C](c/multiples_of_3_or_5_1.c) | Existing report | rust, forth, factor |
| 2 | [Even Fibonacci Numbers](https://projecteuler.net/problem=2) | [C](c/even_fibonacci_numbers_2.c) | Existing report | rust, forth, factor |
| 3 | [Largest Prime Factor](https://projecteuler.net/problem=3) | [C](c/largest_prime_factor_3.c) | Existing report | rust, forth, factor |
| 4 | [Largest Palindrome Product](https://projecteuler.net/problem=4) | [C](c/largest_palindrome_product_4.c) | Existing report | rust, forth, factor |
| 5 | [Smallest Multiple](https://projecteuler.net/problem=5) | [C](c/smallest_multiple_5.c) | Existing report | rust, forth, factor |
| 6 | [Sum Square Difference](https://projecteuler.net/problem=6) | [C](c/sum_square_difference_6.c) | Existing report | rust, forth, factor |
| 7 | [10 001st Prime](https://projecteuler.net/problem=7) | [C](c/10_001st_prime_7.c) | Existing report | rust, forth, factor |
| 8 | [Largest Product in a Series](https://projecteuler.net/problem=8) | [C](c/largest_product_in_a_series_8.c) | Existing report | rust, forth, factor |
| 9 | [Special Pythagorean Triplet](https://projecteuler.net/problem=9) | [C](c/special_pythagorean_triplet_9.c) | Existing report | rust, forth, factor |
| 10 | [Summation of Primes](https://projecteuler.net/problem=10) | [C](c/summation_of_primes_10.c) | Existing report | rust, forth, factor |
| 11 | [Largest Product in a Grid](https://projecteuler.net/problem=11) | [C](c/largest_product_in_a_grid_11.c) | Existing report | rust, forth, factor |
| 12 | [Highly Divisible Triangular Number](https://projecteuler.net/problem=12) | [C](c/highly_divisible_triangular_number_12.c) | Existing report | rust, forth, factor |
| 13 | [Large Sum](https://projecteuler.net/problem=13) | [C](c/large_sum_13.c) | Existing report | rust, forth, factor |
| 14 | [Longest Collatz Sequence](https://projecteuler.net/problem=14) | [C](c/longest_collatz_sequence_14.c) | Existing report | rust, forth, factor |
| 15 | [Lattice Paths](https://projecteuler.net/problem=15) | [C](c/lattice_paths_15.c) | Added | rust, forth, factor |
| 16 | [Power Digit Sum](https://projecteuler.net/problem=16) | [C](c/power_digit_sum_16.c) | Existing report | rust, forth, factor |
| 17 | [Number Letter Counts](https://projecteuler.net/problem=17) | [C](c/number_letter_counts_17.c) | Existing report | rust, forth, factor |
| 18 | [Maximum Path Sum I](https://projecteuler.net/problem=18) | [C](c/maximum_path_sum_i_18.c) | Added | rust, forth, factor |
| 19 | [Counting Sundays](https://projecteuler.net/problem=19) | [C](c/counting_sundays_19.c) | Existing report | rust, forth, factor |
| 20 | [Factorial Digit Sum](https://projecteuler.net/problem=20) | [C](c/factorial_digit_sum_20.c) | Existing report | rust, forth, factor |
| 21 | [Amicable Numbers](https://projecteuler.net/problem=21) | [C](c/amicable_numbers_21.c) | Existing report | rust, forth, factor |
| 22 | [Names Scores](https://projecteuler.net/problem=22) | [C](c/names_scores_22.c) | Existing report | rust, forth, factor |
| 23 | [Non-Abundant Sums](https://projecteuler.net/problem=23) | [C](c/non_abundant_sums_23.c) | Added | rust, forth, factor |
| 24 | [Lexicographic Permutations](https://projecteuler.net/problem=24) | [C](c/lexicographic_permutations_24.c) | Added | rust, forth, factor |
| 25 | [1000-digit Fibonacci Number](https://projecteuler.net/problem=25) | [C](c/1000_digit_fibonacci_number_25.c) | Existing report | rust, forth, factor |
| 26 | [Reciprocal Cycles](https://projecteuler.net/problem=26) | [C](c/reciprocal_cycles_26.c) | Added | rust, forth, factor |
| 27 | [Quadratic Primes](https://projecteuler.net/problem=27) | [C](c/quadratic_primes_27.c) | Existing report | rust, forth, factor |
| 28 | [Number Spiral Diagonals](https://projecteuler.net/problem=28) | [C](c/number_spiral_diagonals_28.c) | Existing report | rust, forth, factor |
| 29 | [Distinct Powers](https://projecteuler.net/problem=29) | [C](c/distinct_powers_29.c) | Existing report | rust, forth, factor |
| 30 | [Digit Fifth Powers](https://projecteuler.net/problem=30) | [C](c/digit_fifth_powers_30.c) | Existing report | rust, forth, factor |
| 31 | [Coin Sums](https://projecteuler.net/problem=31) | [C](c/coin_sums_31.c) | Added | rust, forth, factor |
| 32 | [Pandigital Products](https://projecteuler.net/problem=32) | [C](c/pandigital_products_32.c) | Added | rust, forth, factor |
| 33 | [Digit Cancelling Fractions](https://projecteuler.net/problem=33) | [C](c/digit_cancelling_fractions_33.c) | Added | rust, forth, factor |
| 34 | [Digit Factorials](https://projecteuler.net/problem=34) | [C](c/digit_factorials_34.c) | Existing report | rust, forth, factor |
| 35 | [Circular Primes](https://projecteuler.net/problem=35) | [C](c/circular_primes_35.c) | Existing report | rust, forth, factor |
| 36 | [Double-base Palindromes](https://projecteuler.net/problem=36) | [C](c/double_base_palindromes_36.c) | Added | rust, forth, factor |
| 37 | [Truncatable Primes](https://projecteuler.net/problem=37) | [C](c/truncatable_primes_37.c) | Added | rust, forth, factor |
| 38 | [Pandigital Multiples](https://projecteuler.net/problem=38) | [C](c/pandigital_multiples_38.c) | Added | rust, forth, factor |
| 39 | [Integer Right Triangles](https://projecteuler.net/problem=39) | [C](c/integer_right_triangles_39.c) | Existing report | rust, forth, factor |
| 40 | [Champernowne's Constant](https://projecteuler.net/problem=40) | [C](c/champernownes_constant_40.c) | Existing report | rust, forth, factor |
| 41 | [Pandigital Prime](https://projecteuler.net/problem=41) | [C](c/pandigital_prime_41.c) | Added | rust, forth, factor |
| 42 | [Coded Triangle Numbers](https://projecteuler.net/problem=42) | [C](c/coded_triangle_numbers_42.c) | Added | rust, forth, factor |
| 43 | [Sub-string Divisibility](https://projecteuler.net/problem=43) | [C](c/sub_string_divisibility_43.c) | Added | rust, forth, factor |
| 44 | [Pentagon Numbers](https://projecteuler.net/problem=44) | [C](c/pentagon_numbers_44.c) | Existing report | rust, forth, factor |
| 45 | [Triangular, Pentagonal, and Hexagonal](https://projecteuler.net/problem=45) | [C](c/triangular_pentagonal_and_hexagonal_45.c) | Added | rust, forth, factor |
| 46 | [Goldbach's Other Conjecture](https://projecteuler.net/problem=46) | [C](c/goldbachs_other_conjecture_46.c) | Existing report | rust, forth, factor |
| 47 | [Distinct Primes Factors](https://projecteuler.net/problem=47) | [C](c/distinct_primes_factors_47.c) | Existing report | rust, forth, factor |
| 48 | [Self Powers](https://projecteuler.net/problem=48) | [C](c/self_powers_48.c) | Existing report | rust, forth, factor |
| 49 | [Prime Permutations](https://projecteuler.net/problem=49) | [C](c/prime_permutations_49.c) | Added | rust, forth, factor |
| 50 | [Consecutive Prime Sum](https://projecteuler.net/problem=50) | [C](c/consecutive_prime_sum_50.c) | Existing report | rust, forth, factor |
| 51 | [Prime Digit Replacements](https://projecteuler.net/problem=51) | [C](c/prime_digit_replacements_51.c) | Added | rust, forth, factor |
| 52 | [Permuted Multiples](https://projecteuler.net/problem=52) | [C](c/permuted_multiples_52.c) | Added | rust, forth, factor |
| 53 | [Combinatoric Selections](https://projecteuler.net/problem=53) | [C](c/combinatoric_selections_53.c) | Added | rust, forth, factor |
| 54 | [Poker Hands](https://projecteuler.net/problem=54) | [C](c/poker_hands_54.c) | Added | rust, forth, factor |
| 55 | [Lychrel Numbers](https://projecteuler.net/problem=55) | [C](c/lychrel_numbers_55.c) | Existing report | rust, forth, factor |
| 56 | [Powerful Digit Sum](https://projecteuler.net/problem=56) | [C](c/powerful_digit_sum_56.c) | Added | rust, forth, factor |
| 57 | [Square Root Convergents](https://projecteuler.net/problem=57) | [C](c/square_root_convergents_57.c) | Added | rust, forth, factor |
| 58 | [Spiral Primes](https://projecteuler.net/problem=58) | [C](c/spiral_primes_58.c) | Added | rust, forth, factor |
| 59 | [XOR Decryption](https://projecteuler.net/problem=59) | [C](c/xor_decryption_59.c) | Added | rust, forth, factor |
| 60 | [Prime Pair Sets](https://projecteuler.net/problem=60) | [C](c/prime_pair_sets_60.c) | Added | rust, forth, factor |
| 61 | [Cyclical Figurate Numbers](https://projecteuler.net/problem=61) | [C](c/cyclical_figurate_numbers_61.c) | Added | rust, forth, factor |
| 62 | [Cubic Permutations](https://projecteuler.net/problem=62) | [C](c/cubic_permutations_62.c) | Added | rust, forth, factor |
| 63 | [Powerful Digit Counts](https://projecteuler.net/problem=63) | [C](c/powerful_digit_counts_63.c) | Added | rust, forth, factor |
| 64 | [Odd Period Square Roots](https://projecteuler.net/problem=64) | [C](c/odd_period_square_roots_64.c) | Added | rust, forth, factor |
| 65 | [Convergents of e](https://projecteuler.net/problem=65) | [C](c/convergents_of_e_65.c) | Added | rust, forth, factor |
| 66 | [Diophantine Equation](https://projecteuler.net/problem=66) | [C](c/diophantine_equation_66.c) | Added | rust, forth, factor |
| 67 | [Maximum Path Sum II](https://projecteuler.net/problem=67) | [C](c/maximum_path_sum_ii_67.c) | Added | rust, forth, factor |
| 68 | [Magic 5-gon Ring](https://projecteuler.net/problem=68) | [C](c/magic_5_gon_ring_68.c) | Added | rust, forth, factor |
| 69 | [Totient Maximum](https://projecteuler.net/problem=69) | [C](c/totient_maximum_69.c) | Existing report | rust, forth, factor |
| 70 | [Totient Permutation](https://projecteuler.net/problem=70) | [C](c/totient_permutation_70.c) | Added | rust, forth, factor |
| 71 | [Ordered Fractions](https://projecteuler.net/problem=71) | [C](c/ordered_fractions_71.c) | Added | rust, forth, factor |
| 72 | [Counting Fractions](https://projecteuler.net/problem=72) | [C](c/counting_fractions_72.c) | Added | rust, forth, factor |
| 73 | [Counting Fractions in a Range](https://projecteuler.net/problem=73) | [C](c/counting_fractions_in_a_range_73.c) | Added | rust, forth, factor |
| 74 | [Digit Factorial Chains](https://projecteuler.net/problem=74) | [C](c/digit_factorial_chains_74.c) | Added | rust, forth, factor |
| 75 | [Singular Integer Right Triangles](https://projecteuler.net/problem=75) | [C](c/singular_integer_right_triangles_75.c) | Added | rust, forth, factor |
| 76 | [Counting Summations](https://projecteuler.net/problem=76) | [C](c/counting_summations_76.c) | Added | rust, forth, factor |
| 77 | [Prime Summations](https://projecteuler.net/problem=77) | [C](c/prime_summations_77.c) | Added | rust, forth, factor |
| 78 | [Coin Partitions](https://projecteuler.net/problem=78) | [C](c/coin_partitions_78.c) | Added | rust, forth, factor |
| 79 | [Passcode Derivation](https://projecteuler.net/problem=79) | [C](c/passcode_derivation_79.c) | Added | rust, forth, factor |
| 80 | [Square Root Digital Expansion](https://projecteuler.net/problem=80) | [C](c/square_root_digital_expansion_80.c) | Added | rust, forth, factor |
| 81 | [Path Sum: Two Ways](https://projecteuler.net/problem=81) | [C](c/path_sum_two_ways_81.c) | Added | rust, forth, factor |
| 82 | [Path Sum: Three Ways](https://projecteuler.net/problem=82) | [C](c/path_sum_three_ways_82.c) | Added | rust, forth, factor |
| 83 | [Path Sum: Four Ways](https://projecteuler.net/problem=83) | [C](c/path_sum_four_ways_83.c) | Added | rust, forth, factor |
| 84 | [Monopoly Odds](https://projecteuler.net/problem=84) | [C](c/monopoly_odds_84.c) | Added | rust, forth, factor |
| 85 | [Counting Rectangles](https://projecteuler.net/problem=85) | [C](c/counting_rectangles_85.c) | Added | rust, forth, factor |
| 86 | [Cuboid Route](https://projecteuler.net/problem=86) | [C](c/cuboid_route_86.c) | Added | rust, forth, factor |
| 87 | [Prime Power Triples](https://projecteuler.net/problem=87) | [C](c/prime_power_triples_87.c) | Added | rust, forth, factor |
| 88 | [Product-sum Numbers](https://projecteuler.net/problem=88) | [C](c/product_sum_numbers_88.c) | Added | rust, forth, factor |
| 89 | [Roman Numerals](https://projecteuler.net/problem=89) | [C](c/roman_numerals_89.c) | Added | rust, forth, factor |
| 90 | [Cube Digit Pairs](https://projecteuler.net/problem=90) | [C](c/cube_digit_pairs_90.c) | Added | rust, forth, factor |
| 91 | [Right Triangles with Integer Coordinates](https://projecteuler.net/problem=91) | [C](c/right_triangles_with_integer_coordinates_91.c) | Added | rust, forth, factor |
| 92 | [Square Digit Chains](https://projecteuler.net/problem=92) | [C](c/square_digit_chains_92.c) | Existing report | rust, forth, factor |
| 93 | [Arithmetic Expressions](https://projecteuler.net/problem=93) | [C](c/arithmetic_expressions_93.c) | Added | rust, forth, factor |
| 94 | [Almost Equilateral Triangles](https://projecteuler.net/problem=94) | [C](c/almost_equilateral_triangles_94.c) | Added | rust, forth, factor |
| 95 | [Amicable Chains](https://projecteuler.net/problem=95) | [C](c/amicable_chains_95.c) | Added | rust, forth, factor |
| 96 | [Su Doku](https://projecteuler.net/problem=96) | [C](c/su_doku_96.c) | Added | rust, forth, factor |
| 97 | [Large Non-Mersenne Prime](https://projecteuler.net/problem=97) | [C](c/large_non_mersenne_prime_97.c) | Added | rust, forth, factor |
| 98 | [Anagramic Squares](https://projecteuler.net/problem=98) | [C](c/anagramic_squares_98.c) | Added | rust, forth, factor |
| 99 | [Largest Exponential](https://projecteuler.net/problem=99) | [C](c/largest_exponential_99.c) | Added | rust, forth, factor |
| 100 | [Arranged Probability](https://projecteuler.net/problem=100) | [C](c/arranged_probability_100.c) | Added | rust, forth, factor |
| 101 | [Optimum Polynomial](https://projecteuler.net/problem=101) | [C](c/optimum_polynomial_101.c) | Added | rust, forth, factor |
| 102 | [Triangle Containment](https://projecteuler.net/problem=102) | [C](c/triangle_containment_102.c) | Added | rust, forth, factor |
| 103 | [Special Subset Sums: Optimum](https://projecteuler.net/problem=103) | [C](c/special_subset_sums_optimum_103.c) | Added | rust, forth, factor |
| 104 | [Pandigital Fibonacci Ends](https://projecteuler.net/problem=104) | [C](c/pandigital_fibonacci_ends_104.c) | Added | rust, forth, factor |
| 105 | [Special Subset Sums: Testing](https://projecteuler.net/problem=105) | [C](c/special_subset_sums_testing_105.c) | Added | rust, forth, factor |
| 106 | [Special Subset Sums: Meta-testing](https://projecteuler.net/problem=106) | [C](c/special_subset_sums_meta_testing_106.c) | Added | rust, forth, factor |
| 107 | [Minimal Network](https://projecteuler.net/problem=107) | [C](c/minimal_network_107.c) | Added | rust, forth, factor |
| 108 | [Diophantine Reciprocals I](https://projecteuler.net/problem=108) | [C](c/diophantine_reciprocals_i_108.c) | Added | rust, forth, factor |
| 109 | [Darts](https://projecteuler.net/problem=109) | [C](c/darts_109.c) | Added | rust, forth, factor |
| 110 | [Diophantine Reciprocals II](https://projecteuler.net/problem=110) | [C](c/diophantine_reciprocals_ii_110.c) | Added | rust, forth, factor |
| 111 | [Primes with Runs](https://projecteuler.net/problem=111) | [C](c/primes_with_runs_111.c) | Added | rust, forth, factor |
| 112 | [Bouncy Numbers](https://projecteuler.net/problem=112) | [C](c/bouncy_numbers_112.c) | Added | rust, forth, factor |
| 113 | [Non-bouncy Numbers](https://projecteuler.net/problem=113) | [C](c/non_bouncy_numbers_113.c) | Added | rust, forth, factor |
| 114 | [Counting Block Combinations I](https://projecteuler.net/problem=114) | [C](c/counting_block_combinations_i_114.c) | Added | rust, forth, factor |
| 115 | [Counting Block Combinations II](https://projecteuler.net/problem=115) | [C](c/counting_block_combinations_ii_115.c) | Added | rust, forth, factor |
| 116 | [Red, Green or Blue Tiles](https://projecteuler.net/problem=116) | [C](c/red_green_or_blue_tiles_116.c) | Added | rust, forth, factor |
| 117 | [Red, Green, and Blue Tiles](https://projecteuler.net/problem=117) | [C](c/red_green_and_blue_tiles_117.c) | Added | rust, forth, factor |
| 118 | [Pandigital Prime Sets](https://projecteuler.net/problem=118) | [C](c/pandigital_prime_sets_118.c) | Added | rust, forth, factor |
| 119 | [Digit Power Sum](https://projecteuler.net/problem=119) | [C](c/digit_power_sum_119.c) | Added | rust, forth, factor |
| 120 | [Square Remainders](https://projecteuler.net/problem=120) | [C](c/square_remainders_120.c) | Added | rust, forth, factor |
