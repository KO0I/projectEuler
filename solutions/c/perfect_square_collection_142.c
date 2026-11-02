/* Project Euler 142: https://projecteuler.net/problem=142 */
#include "euler_extra.h"

static bool square_u64(uint64_t n) {
    uint64_t root = (uint64_t)sqrt((double)n);
    while ((root + 1) * (root + 1) <= n) ++root;
    while (root * root > n) --root;
    return root * root == n;
}

int main(void) {
    uint64_t best = UINT64_MAX;
    for (uint64_t ar = 2; ar * ar < best; ++ar) {
        uint64_t a = ar * ar;
        for (uint64_t br = (ar & 1) ? 1 : 2; br < ar; br += 2) {
            uint64_t b = br * br, x = (a + b) / 2, y = (a - b) / 2;
            if (x + y >= best) continue;
            for (uint64_t dr = 1; dr * dr < x; ++dr) {
                uint64_t d = dr * dr, c = 2 * x - d;
                if (!square_u64(c)) continue;
                uint64_t z = x - d;
                if (z && y > z && square_u64(y + z) && square_u64(y - z)) {
                    uint64_t sum = x + y + z;
                    if (sum < best) best = sum;
                }
            }
        }
    }
    printf("%" PRIu64 "\n", best);
    return 0;
}
