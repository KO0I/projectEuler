#include <math.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 195: inscribed circles of 60-degree triangles.
 *
 * Primitive integer solutions of c^2 = a^2 + b^2 - ab are parametrized by
 * coprime m > n.  Swapping n and m-n describes the same triangle, so m > 2n
 * chooses one representative.  Put q = n(m-n).  The primitive inradius is
 *
 *   sqrt(3) q / 2,  normally;
 *   sqrt(3) q / 6,  when m+n is divisible by 3,
 *
 * because the raw Eisenstein-integer parametrization has a common factor 3
 * in the second case.  Every positive integer scaling is another triangle,
 * hence a primitive pair contributes floor(C/(q sqrt(3))) with C=2N or 6N.
 *
 * A floating square root supplies only an estimate.  The correction loops
 * enforce 3(qk)^2 <= C^2 using 128-bit integers, so a value close to an
 * integer boundary cannot introduce an off-by-one error.
 */

static int gcd_positive(int a, int b) {
    while (b != 0) {
        const int remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

static int scale_count(int64_t c, int64_t q) {
    int64_t k = (int64_t)((long double)c / ((long double)q * sqrtl(3.0L)));
    const __int128 c_squared = (__int128)c * c;
    while ((__int128)3 * q * q * (k + 1) * (k + 1) <= c_squared) {
        ++k;
    }
    while ((__int128)3 * q * q * k * k > c_squared) {
        --k;
    }
    return (int)k;
}

static uint64_t triangle_count(int64_t radius_limit) {
    /* The more generous divisible-by-three family has q <= 2*sqrt(3)*N.
       Since m>2n implies q=n(m-n)>n^2, this also bounds n. */
    const int64_t max_q = (int64_t)(2.0L * sqrtl(3.0L) * radius_limit);
    uint64_t total = 0;
    for (int64_t n = 1; n * n < max_q; ++n) {
        const int64_t maximum_m = n + max_q / n;
        for (int64_t m = 2 * n + 1; m <= maximum_m; ++m) {
            if (gcd_positive((int)m, (int)n) != 1) {
                continue;
            }
            const int64_t q = n * (m - n);
            const int64_t c = (m + n) % 3 == 0 ? 6 * radius_limit : 2 * radius_limit;
            total += (uint64_t)scale_count(c, q);
        }
    }
    return total;
}

int main(void) {
    printf("%llu\n", (unsigned long long)triangle_count(1053779));
    return 0;
}
