/* Project Euler 31: https://projecteuler.net/problem=31
 * Processing coin values outside the amount loop counts multisets once.
 * ways[s] includes zero or more copies of every coin processed so far. */
#include "euler_extra.h"

int main(void) {
    static const unsigned coins[] = {1,2,5,10,20,50,100,200};
    uint64_t ways[201] = {1};
    for (size_t i = 0; i < sizeof coins / sizeof *coins; ++i)
        for (unsigned amount = coins[i]; amount <= 200; ++amount)
            ways[amount] += ways[amount - coins[i]];
    printf("%" PRIu64 "\n", ways[200]);
    return 0;
}
