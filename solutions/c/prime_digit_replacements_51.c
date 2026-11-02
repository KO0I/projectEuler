/* Project Euler 51: https://projecteuler.net/problem=51
 * Search increasing digit lengths, then primes. A replacement mask may
 * select only positions that originally contain the same digit. Test all
 * ten replacements, excluding a leading zero, using a full-length sieve. */
#include "euler_extra.h"

int main(void) {
    unsigned lower = 10, upper = 99;
    for (unsigned length = 2; length < 10; ++length) {
        unsigned char *prime = euler_sieve(upper);
        for (unsigned n = lower; n <= upper; ++n) if (prime[n]) {
            unsigned digits[10], powers[10], value = n, power = 1;
            for (unsigned i = 0; i < length; ++i) {
                digits[i] = value % 10; value /= 10;
                powers[i] = power; power *= 10;
            }
            for (unsigned mask = 1; mask < (1u << length); ++mask) {
                unsigned selected_digit = 10, coefficient = 0;
                bool consistent = true;
                for (unsigned i = 0; i < length; ++i) if (mask & (1u << i)) {
                    if (selected_digit == 10) selected_digit = digits[i];
                    else if (selected_digit != digits[i]) { consistent = false; break; }
                    coefficient += powers[i];
                }
                if (!consistent) continue;
                unsigned base = n - selected_digit * coefficient;
                unsigned count = 0, first = 0;
                for (unsigned replacement = 0; replacement < 10; ++replacement) {
                    if (!replacement && (mask & (1u << (length - 1)))) continue;
                    unsigned candidate = base + replacement * coefficient;
                    if (prime[candidate]) {
                        if (!count) first = candidate;
                        ++count;
                    }
                }
                if (count == 8 && first == n) {
                    printf("%u\n", n);
                    free(prime);
                    return 0;
                }
            }
        }
        free(prime);
        lower = upper + 1;
        if (upper > (UINT_MAX - 9) / 10) break;
        upper = 10 * upper + 9;
    }
    euler_fail("No eight-prime family within the unsigned search range");
    return 1;
}
