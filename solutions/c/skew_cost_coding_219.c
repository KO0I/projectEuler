#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 219: skew-cost coding.
 *
 * Codewords are leaves of a binary tree whose edges cost 1 and 4.  Expanding
 * a leaf of cost c replaces it by leaves c+1 and c+4, increasing the leaf
 * count by one and total cost by c+5.  The optimal greedy choice is therefore
 * always the cheapest available leaf.
 *
 * The number f(c) of tree nodes at cost c obeys f(c)=f(c-1)+f(c-4), because
 * those are its two possible parents.  To obtain n leaves we choose n-1
 * internal nodes, namely the n-1 cheapest nodes in this cost-layer sequence.
 * Their costs plus the constant 5 per expansion give the answer without a
 * billion-element priority queue.
 */

int main(void) {
    const uint64_t leaves = UINT64_C(1000000000);
    const uint64_t needed = leaves - 1;
    uint64_t multiplicity[128] = {0};
    multiplicity[0] = 1;

    uint64_t used = 0;
    uint64_t internal_cost = 0;
    for (int cost = 0; used < needed; ++cost) {
        if (cost > 0) {
            multiplicity[cost] = multiplicity[cost - 1];
            if (cost >= 4) {
                multiplicity[cost] += multiplicity[cost - 4];
            }
        }
        const uint64_t remaining = needed - used;
        const uint64_t take = multiplicity[cost] < remaining ? multiplicity[cost] : remaining;
        used += take;
        internal_cost += (uint64_t)cost * take;
    }
    printf("%llu\n", (unsigned long long)(internal_cost + 5 * needed));
    return 0;
}
