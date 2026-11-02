/* Project Euler 207: https://projecteuler.net/problem=207

   Writing k=m(m-1) turns 4^t=2^t+k into m=2^t.  Up through a given m there
   are m-1 partitions and exactly floor(log2(m)) perfect ones.  Scan until the
   strict ratio bound is crossed, then convert m back to k. */
#include "euler_extra.h"
int main(void) {
    uint64_t perfect = 0, next_power = 2;
    for (uint64_t m = 2;; ++m) {
        if (m == next_power) { ++perfect; next_power *= 2; }
        if (perfect * 12345 < m - 1) { printf("%" PRIu64 "\n", m * (m - 1)); return 0; }
    }
}
