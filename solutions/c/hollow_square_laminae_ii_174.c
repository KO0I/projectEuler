/* Project Euler 174: https://projecteuler.net/problem=174 */
#include "euler_extra.h"
int main(void) {
    unsigned char *count = calloc(1000001, 1);
    if (!count) return EXIT_FAILURE;
    for (unsigned outer = 3; 4 * (outer - 1) <= 1000000; ++outer) {
        for (unsigned inner = outer - 2; inner >= 1; inner -= 2) {
            unsigned tiles = outer * outer - inner * inner;
            if (tiles > 1000000) break;
            if (count[tiles] < 11) ++count[tiles];
            if (inner <= 2) break;
        }
    }
    unsigned answer = 0;
    for (unsigned n = 1; n <= 1000000; ++n)
        answer += count[n] >= 1 && count[n] <= 10;
    printf("%u\n", answer);
    free(count);
    return 0;
}
