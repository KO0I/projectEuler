# Project Euler C explanations in LaTeX

This directory contains one LaTeX chapter for each of the **37 main C problem files**, in ascending Project Euler problem order, plus a combined compilable document.

- [Combined LaTeX source](project_euler_c_explanations.tex)
- [Individual chapters](chapters/)
- [C source files](../../solutions/c/)
- [Existing LaTeX explanation for problem 17](../../solutions/latex/number_letter_counts_17.tex)

Problems 13 and 17 now have C implementations with chapters describing the historical approach and the corrections in red. The other chapters retain analysis of the historical attempts and add red, explicitly labelled corrections. A revised solution is not considered validated until it passes tests.

Each chapter describes the mathematical goal, the C control flow and data structures, and the observed limitations. **A chapter is not a claim that its program works.** Several historical files are stubs, have undefined behavior, or calculate something other than their nominal problem.

## Build the combined PDF

Run from this directory:

```sh
make
```

Requires a reasonably complete TeX Live installation with `pdflatex`, `amsmath`, `geometry`, `hyperref`, `microtype`, and Latin Modern fonts. The generated PDF is `build/project_euler_c_explanations.pdf`. Build artifacts are excluded by the repository-wide `.gitignore`.

## Chapters

| Problem | Main C source | LaTeX explanation |
| ---: | --- | --- |
| 1 | [multiples_of_3_or_5_1.c](../../solutions/c/multiples_of_3_or_5_1.c) | [multiples_of_3_or_5_1.tex](chapters/multiples_of_3_or_5_1.tex) |
| 2 | [even_fibonacci_numbers_2.c](../../solutions/c/even_fibonacci_numbers_2.c) | [even_fibonacci_numbers_2.tex](chapters/even_fibonacci_numbers_2.tex) |
| 3 | [largest_prime_factor_3.c](../../solutions/c/largest_prime_factor_3.c) | [largest_prime_factor_3.tex](chapters/largest_prime_factor_3.tex) |
| 4 | [largest_palindrome_product_4.c](../../solutions/c/largest_palindrome_product_4.c) | [largest_palindrome_product_4.tex](chapters/largest_palindrome_product_4.tex) |
| 5 | [smallest_multiple_5.c](../../solutions/c/smallest_multiple_5.c) | [smallest_multiple_5.tex](chapters/smallest_multiple_5.tex) |
| 6 | [sum_square_difference_6.c](../../solutions/c/sum_square_difference_6.c) | [sum_square_difference_6.tex](chapters/sum_square_difference_6.tex) |
| 7 | [10_001st_prime_7.c](../../solutions/c/10_001st_prime_7.c) | [10_001st_prime_7.tex](chapters/10_001st_prime_7.tex) |
| 8 | [largest_product_in_a_series_8.c](../../solutions/c/largest_product_in_a_series_8.c) | [largest_product_in_a_series_8.tex](chapters/largest_product_in_a_series_8.tex) |
| 9 | [special_pythagorean_triplet_9.c](../../solutions/c/special_pythagorean_triplet_9.c) | [special_pythagorean_triplet_9.tex](chapters/special_pythagorean_triplet_9.tex) |
| 10 | [summation_of_primes_10.c](../../solutions/c/summation_of_primes_10.c) | [summation_of_primes_10.tex](chapters/summation_of_primes_10.tex) |
| 11 | [largest_product_in_a_grid_11.c](../../solutions/c/largest_product_in_a_grid_11.c) | [largest_product_in_a_grid_11.tex](chapters/largest_product_in_a_grid_11.tex) |
| 12 | [highly_divisible_triangular_number_12.c](../../solutions/c/highly_divisible_triangular_number_12.c) | [highly_divisible_triangular_number_12.tex](chapters/highly_divisible_triangular_number_12.tex) |
| 13 | [large_sum_13.c](../../solutions/c/large_sum_13.c) | [large_sum_13.tex](chapters/large_sum_13.tex) |
| 14 | [longest_collatz_sequence_14.c](../../solutions/c/longest_collatz_sequence_14.c) | [longest_collatz_sequence_14.tex](chapters/longest_collatz_sequence_14.tex) |
| 16 | [power_digit_sum_16.c](../../solutions/c/power_digit_sum_16.c) | [power_digit_sum_16.tex](chapters/power_digit_sum_16.tex) |
| 17 | [number_letter_counts_17.c](../../solutions/c/number_letter_counts_17.c) | [number_letter_counts_17.tex](chapters/number_letter_counts_17.tex) |
| 19 | [counting_sundays_19.c](../../solutions/c/counting_sundays_19.c) | [counting_sundays_19.tex](chapters/counting_sundays_19.tex) |
| 20 | [factorial_digit_sum_20.c](../../solutions/c/factorial_digit_sum_20.c) | [factorial_digit_sum_20.tex](chapters/factorial_digit_sum_20.tex) |
| 21 | [amicable_numbers_21.c](../../solutions/c/amicable_numbers_21.c) | [amicable_numbers_21.tex](chapters/amicable_numbers_21.tex) |
| 22 | [names_scores_22.c](../../solutions/c/names_scores_22.c) | [names_scores_22.tex](chapters/names_scores_22.tex) |
| 25 | [1000_digit_fibonacci_number_25.c](../../solutions/c/1000_digit_fibonacci_number_25.c) | [1000_digit_fibonacci_number_25.tex](chapters/1000_digit_fibonacci_number_25.tex) |
| 27 | [quadratic_primes_27.c](../../solutions/c/quadratic_primes_27.c) | [quadratic_primes_27.tex](chapters/quadratic_primes_27.tex) |
| 28 | [number_spiral_diagonals_28.c](../../solutions/c/number_spiral_diagonals_28.c) | [number_spiral_diagonals_28.tex](chapters/number_spiral_diagonals_28.tex) |
| 29 | [distinct_powers_29.c](../../solutions/c/distinct_powers_29.c) | [distinct_powers_29.tex](chapters/distinct_powers_29.tex) |
| 30 | [digit_fifth_powers_30.c](../../solutions/c/digit_fifth_powers_30.c) | [digit_fifth_powers_30.tex](chapters/digit_fifth_powers_30.tex) |
| 34 | [digit_factorials_34.c](../../solutions/c/digit_factorials_34.c) | [digit_factorials_34.tex](chapters/digit_factorials_34.tex) |
| 35 | [circular_primes_35.c](../../solutions/c/circular_primes_35.c) | [circular_primes_35.tex](chapters/circular_primes_35.tex) |
| 39 | [integer_right_triangles_39.c](../../solutions/c/integer_right_triangles_39.c) | [integer_right_triangles_39.tex](chapters/integer_right_triangles_39.tex) |
| 40 | [champernownes_constant_40.c](../../solutions/c/champernownes_constant_40.c) | [champernownes_constant_40.tex](chapters/champernownes_constant_40.tex) |
| 44 | [pentagon_numbers_44.c](../../solutions/c/pentagon_numbers_44.c) | [pentagon_numbers_44.tex](chapters/pentagon_numbers_44.tex) |
| 46 | [goldbachs_other_conjecture_46.c](../../solutions/c/goldbachs_other_conjecture_46.c) | [goldbachs_other_conjecture_46.tex](chapters/goldbachs_other_conjecture_46.tex) |
| 47 | [distinct_primes_factors_47.c](../../solutions/c/distinct_primes_factors_47.c) | [distinct_primes_factors_47.tex](chapters/distinct_primes_factors_47.tex) |
| 48 | [self_powers_48.c](../../solutions/c/self_powers_48.c) | [self_powers_48.tex](chapters/self_powers_48.tex) |
| 50 | [consecutive_prime_sum_50.c](../../solutions/c/consecutive_prime_sum_50.c) | [consecutive_prime_sum_50.tex](chapters/consecutive_prime_sum_50.tex) |
| 55 | [lychrel_numbers_55.c](../../solutions/c/lychrel_numbers_55.c) | [lychrel_numbers_55.tex](chapters/lychrel_numbers_55.tex) |
| 69 | [totient_maximum_69.c](../../solutions/c/totient_maximum_69.c) | [totient_maximum_69.tex](chapters/totient_maximum_69.tex) |
| 92 | [square_digit_chains_92.c](../../solutions/c/square_digit_chains_92.c) | [square_digit_chains_92.tex](chapters/square_digit_chains_92.tex) |

Where multiple C versions exist, chapters describe the main C source linked above. Alternative versions remain under `solutions/c/variants/`.
