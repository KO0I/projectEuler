/* https://projecteuler.net/problem=90
 * Enumerate the 210 six-element digit sets and unordered pairs of them.
 * Keep 6-labelled and 9-labelled cubes distinct, but allow either symbol to
 * display both digits when testing the nine two-digit square numbers. */
#include "euler_extra.h"

static unsigned p90_popcount(unsigned n) {
    unsigned count = 0;
    while (n) { n &= n - 1; ++count; }
    return count;
}

static bool p90_valid(unsigned left, unsigned right) {
    static const unsigned squares[] = {1, 4, 9, 16, 25, 36, 49, 64, 81};
    const unsigned reversible = (1u << 6) | (1u << 9);
    if (left & reversible) left |= reversible;
    if (right & reversible) right |= reversible;
    for (size_t i = 0; i < sizeof squares / sizeof *squares; ++i) {
        unsigned a = 1u << (squares[i] / 10), b = 1u << (squares[i] % 10);
        if (!((left & a) && (right & b)) && !((left & b) && (right & a))) return false;
    }
    return true;
}

int main(void) {
    unsigned masks[210], count = 0, answer = 0;
    for (unsigned mask = 0; mask < 1024; ++mask) if (p90_popcount(mask) == 6) masks[count++] = mask;
    for (unsigned i = 0; i < count; ++i)
        for (unsigned j = i; j < count; ++j) answer += p90_valid(masks[i], masks[j]);
    printf("%u\n", answer);
    return 0;
}
