#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project Euler 196: prime triplets.
 *
 * Number (row,column), with zero-based column, is T(row-1)+column+1.
 * Its eight possible neighbours are the two horizontal entries and columns
 * column-1..column+1 in each adjacent row.  A prime belongs to a triplet if
 * either it has two prime neighbours, or one of its prime neighbours has two
 * prime neighbours.  Testing the latter can reach two rows away, so S(n)
 * needs primality only for the five-row strip n-2 through n+2.
 *
 * The target values are around 10^13, far beyond an ordinary sieve array.
 * A segmented sieve marks the approximately 5n values in the strip using
 * base primes only through sqrt(the strip's end).  The two requested rows are
 * processed independently, keeping peak memory near 40 MB rather than the
 * size of the entire triangular sequence.
 */

typedef struct {
    uint64_t low;
    uint64_t high;
    unsigned char *prime;
} PrimeStrip;

static uint64_t row_start(int64_t row) {
    return (uint64_t)row * (uint64_t)(row - 1) / 2 + 1;
}

static uint64_t row_end(int64_t row) {
    return (uint64_t)row * (uint64_t)(row + 1) / 2;
}

static int build_base_primes(int limit, int **primes_out) {
    unsigned char *composite = calloc((size_t)limit + 1, 1);
    int *primes = malloc(((size_t)limit + 1) * sizeof(*primes));
    if (composite == NULL || primes == NULL) {
        free(primes);
        free(composite);
        return -1;
    }
    int count = 0;
    for (int value = 2; value <= limit; ++value) {
        if (!composite[value]) {
            primes[count++] = value;
            if ((int64_t)value * value <= limit) {
                for (int multiple = value * value; multiple <= limit; multiple += value) {
                    composite[multiple] = 1;
                }
            }
        }
    }
    free(composite);
    *primes_out = primes;
    return count;
}

static int make_strip(int64_t row, const int *base_primes, int base_count,
                      PrimeStrip *strip) {
    strip->low = row_start(row - 2);
    strip->high = row_end(row + 2);
    const size_t length = (size_t)(strip->high - strip->low + 1);
    strip->prime = malloc(length);
    if (strip->prime == NULL) {
        return 0;
    }
    memset(strip->prime, 1, length);

    for (int i = 0; i < base_count; ++i) {
        const uint64_t p = (uint64_t)base_primes[i];
        if (p * p > strip->high) {
            break;
        }
        uint64_t first = (strip->low + p - 1) / p * p;
        if (first < p * p) {
            first = p * p;
        }
        for (uint64_t value = first; value <= strip->high; value += p) {
            strip->prime[value - strip->low] = 0;
        }
    }
    return 1;
}

static int is_prime_at(const PrimeStrip *strip, int64_t row, int64_t column) {
    if (row < 1 || column < 0 || column >= row) {
        return 0;
    }
    const uint64_t value = row_start(row) + (uint64_t)column;
    return value >= strip->low && value <= strip->high && strip->prime[value - strip->low];
}

static int prime_neighbour_count(const PrimeStrip *strip, int64_t row, int64_t column) {
    int count = 0;
    count += is_prime_at(strip, row, column - 1);
    count += is_prime_at(strip, row, column + 1);
    for (int delta = -1; delta <= 1; ++delta) {
        count += is_prime_at(strip, row - 1, column + delta);
        count += is_prime_at(strip, row + 1, column + delta);
    }
    return count;
}

static int is_triplet_member(const PrimeStrip *strip, int64_t row, int64_t column) {
    if (prime_neighbour_count(strip, row, column) >= 2) {
        return 1;
    }

    /* If this prime is not the centre, it can still be one of the two leaves
       of a triplet centred at any adjacent prime. */
    static const int row_delta[8] = {0, 0, -1, -1, -1, 1, 1, 1};
    static const int col_delta[8] = {-1, 1, -1, 0, 1, -1, 0, 1};
    for (int i = 0; i < 8; ++i) {
        const int64_t other_row = row + row_delta[i];
        const int64_t other_col = column + col_delta[i];
        if (is_prime_at(strip, other_row, other_col) &&
            prime_neighbour_count(strip, other_row, other_col) >= 2) {
            return 1;
        }
    }
    return 0;
}

static uint64_t row_sum(int64_t row, const int *base_primes, int base_count) {
    PrimeStrip strip;
    if (!make_strip(row, base_primes, base_count, &strip)) {
        return 0;
    }
    uint64_t sum = 0;
    const uint64_t first = row_start(row);
    for (int64_t column = 0; column < row; ++column) {
        const uint64_t value = first + (uint64_t)column;
        if (strip.prime[value - strip.low] && is_triplet_member(&strip, row, column)) {
            sum += value;
        }
    }
    free(strip.prime);
    return sum;
}

int main(void) {
    const int64_t row1 = 5678027;
    const int64_t row2 = 7208785;
    const uint64_t maximum = row_end(row2 + 2);
    int limit = (int)sqrtl((long double)maximum) + 1;
    while ((uint64_t)limit * (uint64_t)limit < maximum) {
        ++limit;
    }

    int *base_primes = NULL;
    const int base_count = build_base_primes(limit, &base_primes);
    if (base_count < 0) {
        return 1;
    }
    const uint64_t answer = row_sum(row1, base_primes, base_count) +
                            row_sum(row2, base_primes, base_count);
    printf("%llu\n", (unsigned long long)answer);
    free(base_primes);
    return 0;
}
