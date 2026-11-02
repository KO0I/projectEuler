/* Project Euler 201: https://projecteuler.net/problem=201

   DP[k][s] stores how many ways the processed squares choose k terms totaling
   s, but counts are saturated at two: the final question distinguishes only
   zero, unique, and non-unique.  Descending k and sum prevent reusing the same
   square in one subset.  The maximum possible sum of all 100 squares bounds a
   compact byte table. */
#include "euler_extra.h"
int main(void) {
    enum { CHOOSE = 50, MAX_SUM = 338350 };
    unsigned char *ways = calloc((CHOOSE + 1) * (MAX_SUM + 1), 1);
    if (!ways) return EXIT_FAILURE;
#define W201(k,s) ways[(size_t)(k) * (MAX_SUM + 1) + (s)]
    W201(0,0) = 1;
    unsigned reachable = 0;
    for (unsigned n = 1; n <= 100; ++n) {
        unsigned square = n * n;
        reachable += square;
        unsigned top = n < CHOOSE ? n : CHOOSE;
        for (unsigned k = top; k >= 1; --k) {
            for (unsigned sum = reachable; sum >= square; --sum) {
                unsigned count = W201(k,sum) + W201(k-1,sum-square);
                W201(k,sum) = count > 2 ? 2 : count;
                if (sum == square) break; /* avoid unsigned wrap */
            }
        }
    }
    uint64_t answer = 0;
    for (unsigned sum = 0; sum <= MAX_SUM; ++sum) if (W201(CHOOSE,sum) == 1) answer += sum;
    printf("%" PRIu64 "\n", answer);
    free(ways);
    return 0;
}
