/* https://projecteuler.net/problem=75
 * Coprime, opposite-parity m>n give each primitive Pythagorean triangle once,
 * with perimeter 2*m*(m+n). Count its multiples, saturating counts at two. */
#include "euler_extra.h"

static unsigned p75_unique(unsigned limit) {
    unsigned char *counts = euler_calloc((size_t)limit + 1, 1);
    for (unsigned m = 2; 2 * m * (m + 1) <= limit; ++m)
        for (unsigned n = 1; n < m; ++n) {
            if ((m + n) % 2 == 0 || euler_gcd(m, n) != 1) continue;
            unsigned perimeter = 2 * m * (m + n);
            if (perimeter > limit) break;
            for (unsigned p = perimeter; p <= limit; p += perimeter)
                if (counts[p] < 2) ++counts[p];
        }
    unsigned answer = 0;
    for (unsigned p = 1; p <= limit; ++p) answer += counts[p] == 1;
    free(counts);
    return answer;
}

int main(void) {
    printf("%u\n", p75_unique(1500000));
    return 0;
}
