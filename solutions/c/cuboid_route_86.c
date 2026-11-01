/* https://projecteuler.net/problem=86
 * With a<=b<=c the shortest unfolded route is sqrt(c*c+(a+b)^2).
 * For a fixed c and s=a+b, valid a run from max(1,s-c) through floor(s/2).
 * This counts unordered cuboids once without iterating over a and b separately. */
#include "euler_extra.h"

static uint64_t p86_layer(unsigned c) {
    uint64_t count = 0;
    for (uint64_t sum = 2; sum <= 2 * (uint64_t)c; ++sum) {
        uint64_t squared = (uint64_t)c * c + sum * sum;
        uint64_t root = euler_isqrt(squared);
        if (root * root != squared) continue;
        uint64_t low = sum > c ? sum - c : 1, high = sum / 2;
        if (high >= low) count += high - low + 1;
    }
    return count;
}

int main(void) {
    uint64_t count = 0;
    for (unsigned largest = 1;; ++largest) {
        count += p86_layer(largest);
        if (count > 1000000) { printf("%u\n", largest); return 0; }
    }
}
