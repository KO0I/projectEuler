/* Project Euler 181: https://projecteuler.net/problem=181

   A grouping is an unordered multiset of group types.  A type (b,w) says
   that one group contains b black and w white objects.  Iterating the types
   outside the usual coin-change recurrence makes every multiset appear once;
   iterating object totals upward allows a type to be used repeatedly. */
#include "euler_extra.h"

int main(void) {
    uint64_t ways[61][41] = {{0}};
    ways[0][0] = 1;
    for (unsigned black = 0; black <= 60; ++black) {
        for (unsigned white = 0; white <= 40; ++white) {
            if (black == 0 && white == 0) continue; /* an empty group is not a type */
            for (unsigned b = black; b <= 60; ++b)
                for (unsigned w = white; w <= 40; ++w)
                    ways[b][w] += ways[b - black][w - white];
        }
    }
    printf("%" PRIu64 "\n", ways[60][40]);
    return 0;
}
