/* https://projecteuler.net/problem=68
 * A 16-digit ring must put 10 outside, since inner nodes occur twice. Enumerate
 * five distinct inner nodes, then derive every outer node from the common sum.
 * The first outer node is constrained to be the smallest to fix the rotation. */
#include "euler_extra.h"

static int p68_inner[5];
static uint64_t p68_best;

static void p68_search(int depth, unsigned used) {
    if (depth < 5) {
        for (int value = 1; value <= 9; ++value) if (!(used & (1u << value))) {
            p68_inner[depth] = value;
            p68_search(depth + 1, used | (1u << value));
        }
        return;
    }
    for (int first = 1; first <= 10; ++first) if (!(used & (1u << first))) {
        int outer[5], sum = first + p68_inner[0] + p68_inner[1];
        unsigned all = used;
        bool valid = true;
        for (int i = 0; i < 5; ++i) {
            outer[i] = sum - p68_inner[i] - p68_inner[(i + 1) % 5];
            if (outer[i] < first || outer[i] > 10 || (all & (1u << outer[i]))) {
                valid = false; break;
            }
            all |= 1u << outer[i];
        }
        if (!valid) continue;
        uint64_t representation = 0;
        for (int i = 0; i < 5; ++i) {
            representation = representation * (outer[i] == 10 ? 100 : 10) + (unsigned)outer[i];
            representation = representation * 10 + (unsigned)p68_inner[i];
            representation = representation * 10 + (unsigned)p68_inner[(i + 1) % 5];
        }
        if (representation > p68_best) p68_best = representation;
    }
}

int main(void) {
    p68_search(0, 0);
    printf("%" PRIu64 "\n", p68_best);
    return 0;
}
