/* Project Euler 24: https://projecteuler.net/problem=24
 * The remaining permutations split into equal factorial-sized blocks.
 * Unrank zero-based index 999999, deleting each chosen digit. */
#include "euler_extra.h"

int main(void) {
    unsigned factorial[11] = {1}, digits[10];
    for (unsigned i = 1; i <= 10; ++i) factorial[i] = i * factorial[i - 1];
    for (unsigned i = 0; i < 10; ++i) digits[i] = i;
    unsigned rank = 1000000 - 1;
    uint64_t answer = 0;
    for (unsigned remaining = 10; remaining; --remaining) {
        unsigned index = rank / factorial[remaining - 1];
        rank %= factorial[remaining - 1];
        answer = answer * 10 + digits[index];
        for (unsigned i = index; i + 1 < remaining; ++i) digits[i] = digits[i + 1];
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
