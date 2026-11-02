/* Project Euler 205: https://projecteuler.net/problem=205

   Convolution builds the exact frequency distribution for each player's
   dice.  Summing Peter's frequency times Colin's cumulative lower frequency
   gives the favorable outcomes; division occurs only for final formatting. */
#include "euler_extra.h"
static void distribution205(unsigned dice, unsigned sides, uint64_t *result) {
    uint64_t current[64] = {1}, next[64];
    for (unsigned d = 0; d < dice; ++d) {
        memset(next, 0, sizeof(next));
        for (unsigned sum = 0; sum < 63; ++sum)
            for (unsigned face = 1; face <= sides; ++face) next[sum + face] += current[sum];
        memcpy(current, next, sizeof(current));
    }
    memcpy(result, current, sizeof(current));
}
int main(void) {
    uint64_t peter[64], colin[64]; distribution205(9, 4, peter); distribution205(6, 6, colin);
    uint64_t favorable = 0, lower = 0;
    for (unsigned sum = 0; sum < 64; ++sum) { favorable += peter[sum] * lower; lower += colin[sum]; }
    double probability = (double)favorable / (pow(4, 9) * pow(6, 6));
    printf("%.7f\n", probability);
    return 0;
}
