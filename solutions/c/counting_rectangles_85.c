/* https://projecteuler.net/problem=85
 * An m by n grid has T(m)*T(n) rectangles, where T(k)=k*(k+1)/2.
 * Restrict m<=n. For each m, counts increase with n, so stop immediately
 * above the target. Once the square count is too large, every later m loses. */
#include "euler_extra.h"

int main(void) {
    const uint64_t target = 2000000;
    uint64_t best_difference = target, best_area = 0;
    for (uint64_t m = 1;; ++m) {
        uint64_t tm = m * (m + 1) / 2;
        if (tm * tm > target && tm * tm - target > best_difference) break;
        for (uint64_t n = m;; ++n) {
            uint64_t count = tm * n * (n + 1) / 2;
            uint64_t difference = count > target ? count - target : target - count;
            if (difference < best_difference) { best_difference = difference; best_area = m * n; }
            if (count >= target) break;
        }
    }
    printf("%" PRIu64 "\n", best_area);
    return 0;
}
