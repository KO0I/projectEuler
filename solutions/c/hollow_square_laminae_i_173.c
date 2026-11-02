/* Project Euler 173: https://projecteuler.net/problem=173 */
#include "euler_extra.h"
int main(void) {
    unsigned count = 0;
    for (unsigned outer = 3; 4 * (outer - 1) <= 1000000; ++outer) {
        for (unsigned inner = outer - 2; inner >= 1; inner -= 2) {
            if (outer * outer - inner * inner > 1000000) break;
            ++count;
            if (inner <= 2) break;
        }
    }
    printf("%u\n", count);
    return 0;
}
