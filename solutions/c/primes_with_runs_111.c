/* Project Euler 111: https://projecteuler.net/problem=111
 * For each repeated digit, increase the number of replacements from zero.
 * Enumerate every placement and every different replacement digit exactly
 * once, stopping at the first count with at least one prime. */
#include "euler_extra.h"

typedef struct { unsigned length, digit; uint64_t count, sum; } Runs;
static void replacements(Runs *runs, unsigned position, unsigned left, uint64_t value) {
    if (left > runs->length - position) return;
    if (position == runs->length) {
        if (!left && euler_is_prime(value)) { ++runs->count; runs->sum += value; }
        return;
    }
    if (runs->digit || position)
        replacements(runs, position + 1, left, 10 * value + runs->digit);
    if (left) for (unsigned digit = position ? 0 : 1; digit <= 9; ++digit)
        if (digit != runs->digit)
            replacements(runs, position + 1, left - 1, 10 * value + digit);
}
static uint64_t run_sum(unsigned length) {
    uint64_t total = 0;
    for (unsigned digit = 0; digit <= 9; ++digit) {
        Runs runs = {length, digit, 0, 0};
        for (unsigned changed = 0; changed <= length; ++changed) {
            runs.count = runs.sum = 0;
            replacements(&runs, 0, changed, 0);
            if (runs.count) { total += runs.sum; break; }
        }
    }
    return total;
}
int main(void) {
    printf("%" PRIu64 "\n", run_sum(10));
    return 0;
}
