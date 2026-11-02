/* Project Euler 154: Exploring Pascal's Pyramid
 *
 * A trinomial coefficient 200000!/(a!b!c!) is divisible by 10^12 exactly when
 * its prime valuations v2 and v5 are both at least 12.  Prefix tables store
 * v_p(n!), allowing each coefficient's two valuations to be tested in O(1).
 *
 * Choose i=b+c first, then j=b.  If C(200000,i) already supplies both twelve
 * factors, every one of the i+1 splits qualifies.  Otherwise only half the
 * b,c pairs are tested and nonsymmetric successes are doubled.  This symmetry
 * and the fast outer acceptance remove most of the naive quadratic work. */
#include "euler_extra.h"

static unsigned choose154(const unsigned *factorial, unsigned n, unsigned k) {
    return factorial[n] - factorial[k] - factorial[n - k];
}

int main(void) {
    enum { LAYER = 200000 };
    unsigned *twos = calloc(LAYER + 1, sizeof(*twos));
    unsigned *fives = calloc(LAYER + 1, sizeof(*fives));
    if (!twos || !fives) return EXIT_FAILURE;
    for (unsigned value = 1; value <= LAYER; ++value) {
        unsigned current = value;
        twos[value] = twos[value - 1];
        fives[value] = fives[value - 1];
        while (current % 2 == 0) { ++twos[value]; current /= 2; }
        while (current % 5 == 0) { ++fives[value]; current /= 5; }
    }
    uint64_t answer = 0;
    for (unsigned i = 0; i <= LAYER; ++i) {
        unsigned outer2 = choose154(twos, LAYER, i);
        unsigned outer5 = choose154(fives, LAYER, i);
        if (outer2 >= 12 && outer5 >= 12) {
            answer += i + 1;
            continue;
        }
        for (unsigned j = 0; j <= (i + 1) / 2; ++j) {
            if (outer2 + choose154(twos, i, j) >= 12 &&
                outer5 + choose154(fives, i, j) >= 12) {
                ++answer;
                if (j < i / 2) ++answer;
            }
        }
    }
    printf("%" PRIu64 "\n", answer);
    free(twos);
    free(fives);
    return 0;
}
