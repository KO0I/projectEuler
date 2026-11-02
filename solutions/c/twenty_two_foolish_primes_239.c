#include <stdio.h>

/*
 * Project Euler 239: Twenty-two Foolish Primes
 *
 * There are 25 prime-labelled discs.  Exactly 22 away from their natural
 * positions means exactly three prime discs are fixed.  Choose those three in
 * C(25,3) ways.  Of the remaining 97 discs, the other 22 marked discs must
 * avoid their own positions; the 75 non-prime discs are unrestricted.
 * Inclusion-exclusion therefore gives the probability
 *
 *   C(25,3) * sum_{j=0..22} (-1)^j C(22,j) (97-j)! / 100!.
 *
 * Rather than construct enormous factorials, the code advances normalized
 * terms by
 *
 *   term(j+1)/term(j) = -(22-j) / ((j+1)(97-j)).
 *
 * Long double leaves ample guard digits for the alternating sum before the
 * required rounding to twelve decimal places.
 */

int main(void) {
    const long double choose_three = (25.0L * 24.0L * 23.0L) / 6.0L;
    long double term = 1.0L / (100.0L * 99.0L * 98.0L);
    long double sum = term;

    for (int j = 0; j < 22; ++j) {
        term *= -(long double)(22 - j)
                / ((long double)(j + 1) * (97 - j));
        sum += term;
    }

    printf("%.12Lf\n", choose_three * sum);
    return 0;
}
