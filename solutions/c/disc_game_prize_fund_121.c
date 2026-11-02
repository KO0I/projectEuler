/* Project Euler 121: Disc Game Prize Fund
 *
 * On turn t, multiplying every outcome probability by t+1 leaves integer
 * weights: blue contributes 1 and red contributes t.  `ways[r]` stores the
 * total weight of sequences containing r red draws, updated backward so each
 * turn is used once.  The common denominator after 15 turns is 2*3*...*16.
 *
 * Winning means strictly more blue than red, hence fewer than eight reds.
 * If W is the weighted winning count and D the denominator, the largest fair
 * integral prize fund is floor(D/W).  All weights fit uint64_t. */
#include "euler_extra.h"

static uint64_t prize_fund(unsigned turns) {
    uint64_t ways[16] = {1};
    uint64_t denominator = 1;
    for (unsigned turn = 1; turn <= turns; ++turn) {
        for (unsigned reds = turn; reds > 0; --reds)
            ways[reds] += ways[reds - 1] * turn;
        denominator *= turn + 1;
    }
    uint64_t winning = 0;
    for (unsigned reds = 0; reds < (turns + 1) / 2; ++reds) winning += ways[reds];
    return denominator / winning;
}

int main(void) {
    printf("%" PRIu64 "\n", prize_fund(15));
    return 0;
}
