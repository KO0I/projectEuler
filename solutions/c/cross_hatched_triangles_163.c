/* Project Euler 163: https://projecteuler.net/problem=163 */
#include "euler_extra.h"

int main(void) {
    const int64_t n = 36;
    int64_t numerator = 1678 * n * n * n + 3117 * n * n + 88 * n;
    numerator -= 345 * (n % 2);
    numerator -= 320 * (n % 3);
    numerator -= 90 * (n % 4);
    numerator -= 288 * (n % 5);
    printf("%" PRId64 "\n", numerator / 240);
    return 0;
}
