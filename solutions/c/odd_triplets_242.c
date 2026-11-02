#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 242: Odd Triplets
 *
 * Reducing the subset generating function modulo two shows that odd triplets
 * occur only for n=4t+1.  Lucas' theorem then says the admissible odd k values
 * are indexed by binary submasks of t, of which there are 2^popcount(t).
 * Thus the requested count is
 *
 *     sum_{t=0..floor((10^12-1)/4)} 2^popcount(t).
 *
 * A binary digit DP evaluates this without visiting 250 billion t values.
 * At a 1-bit of the upper bound, choosing 0 makes the prefix smaller and frees
 * i lower bits.  A free bit contributes weight 1 when zero and 2 when one, so
 * all i free bits contribute 3^i.  Following the bound's 1-bit doubles the
 * current prefix weight.  Finally the bound itself is added.
 */

int main(void) {
    const uint64_t upper = (UINT64_C(1000000000000) - 1) / 4;
    /* upper < 2^38, and 3^39 still fits in uint64_t. */
    uint64_t powers_of_three[40];
    powers_of_three[0] = 1;
    for (int i = 1; i < 40; ++i)
        powers_of_three[i] = powers_of_three[i - 1] * 3;

    uint64_t answer = 0;
    uint64_t prefix_weight = 1;
    for (int bit = 39; bit >= 0; --bit) {
        if ((upper >> bit) & 1U) {
            answer += prefix_weight * powers_of_three[bit];
            prefix_weight *= 2;
        }
    }
    answer += prefix_weight;

    printf("%llu\n", (unsigned long long)answer);
    return 0;
}
