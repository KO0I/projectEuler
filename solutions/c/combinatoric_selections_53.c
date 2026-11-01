/* Project Euler 53: https://projecteuler.net/problem=53
 * Pascal's recurrence is monotone in its nonnegative inputs. Saturating
 * coefficients at 1000001 preserves precisely the required comparison. */
#include "euler_extra.h"

int main(void) {
    enum { THRESHOLD = 1000000 };
    unsigned row[101] = {1}, count = 0;
    for (unsigned n = 1; n <= 100; ++n) {
        for (unsigned r = n; r; --r) {
            unsigned sum = row[r] + row[r - 1];
            row[r] = sum > THRESHOLD ? THRESHOLD + 1 : sum;
        }
        for (unsigned r = 0; r <= n; ++r) count += row[r] > THRESHOLD;
    }
    printf("%u\n", count);
    return 0;
}
