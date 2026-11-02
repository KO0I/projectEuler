/* Project Euler 206: https://projecteuler.net/problem=206

   The square pattern ends in 900, so its root ends in 30 or 70.  We scan only
   those two residue classes between the square-root bounds and test alternate
   decimal digits from right to left. */
#include "euler_extra.h"
static bool pattern206(uint64_t square) {
    static const unsigned expected[] = {0,9,8,7,6,5,4,3,2,1};
    for (unsigned i = 0; i < 10; ++i) { if (square % 10 != expected[i]) return false; square /= 100; }
    return true;
}
int main(void) {
    for (uint64_t root = 1010101030; root <= 1389026630; root += root % 100 == 30 ? 40 : 60) {
        if (pattern206(root * root)) { printf("%" PRIu64 "\n", root); return 0; }
    }
    return EXIT_FAILURE;
}
