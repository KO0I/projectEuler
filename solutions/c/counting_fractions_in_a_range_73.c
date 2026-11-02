/* https://projecteuler.net/problem=73
 * 1/3 and 1/2 are Farey neighbours. Repeatedly inserting their mediant visits
 * every reduced fraction between them exactly once. Only the denominators
 * are needed to prune a branch when their sum exceeds 12000. */
#include "euler_extra.h"

static uint64_t p73_between(unsigned left_d, unsigned right_d, unsigned limit) {
    unsigned middle_d = left_d + right_d;
    if (middle_d > limit) return 0;
    return 1 + p73_between(left_d, middle_d, limit) + p73_between(middle_d, right_d, limit);
}

int main(void) {
    printf("%" PRIu64 "\n", p73_between(3, 2, 12000));
    return 0;
}
