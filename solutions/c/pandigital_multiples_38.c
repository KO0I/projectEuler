/* Project Euler 38: https://projecteuler.net/problem=38
 * With at least two multiples, the starting integer has at most four
 * digits. Concatenate its successive multiples until nine digits appear. */
#include "euler_extra.h"

int main(void) {
    uint64_t best = 0;
    for (unsigned n = 1; n < 10000; ++n) {
        unsigned used = 0, digits = 0;
        uint64_t joined = 0;
        for (unsigned multiplier = 1; digits < 9; ++multiplier) {
            char text[16];
            snprintf(text, sizeof text, "%u", n * multiplier);
            bool valid = true;
            for (const char *p = text; *p; ++p) {
                unsigned d = (unsigned)(*p - '0'), bit = 1u << d;
                if (!d || (used & bit) || digits == 9) { valid = false; break; }
                used |= bit;
                ++digits;
                joined = joined * 10 + d;
            }
            if (!valid) break;
            if (digits == 9 && multiplier > 1 && joined > best) best = joined;
        }
    }
    printf("%" PRIu64 "\n", best);
    return 0;
}
