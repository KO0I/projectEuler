/* Project Euler 208: https://projecteuler.net/problem=208

   Every arc changes the heading by +/-72 degrees.  The five possible movement
   directions are the fifth roots of unity; a nonnegative integer combination
   of them sums to zero only when all five direction counts are equal.  Thus a
   70-arc closed walk uses every direction 14 times.  The DP records four
   counts and the current heading; the fifth count follows from the step index.
   For a closed 70-arc journey the accumulated turn also restores heading zero,
   so only that terminal heading belongs to the closed-state class. */
#include "euler_extra.h"
enum { SIDE208 = 15, STATES208 = SIDE208*SIDE208*SIDE208*SIDE208*5 };
static size_t index208(unsigned a,unsigned b,unsigned c,unsigned d,unsigned h) {
    return (((((size_t)a*SIDE208+b)*SIDE208+c)*SIDE208+d)*5+h);
}
int main(void) {
    uint64_t *current = calloc(STATES208, sizeof(*current));
    uint64_t *next = calloc(STATES208, sizeof(*next));
    if (!current || !next) return EXIT_FAILURE;
    current[index208(0,0,0,0,0)] = 1;
    for (unsigned step = 0; step < 70; ++step) {
        memset(next, 0, STATES208 * sizeof(*next));
        for (unsigned a=0;a<=14;++a) for(unsigned b=0;b<=14;++b)
        for (unsigned c=0;c<=14;++c) for(unsigned d=0;d<=14;++d) {
            unsigned used = a+b+c+d;
            if (used > step || step-used > 14) continue;
            for (unsigned heading=0;heading<5;++heading) {
                uint64_t ways=current[index208(a,b,c,d,heading)]; if(!ways)continue;
                for(int turn=-1;turn<=1;turn+=2){unsigned h=(heading+5+turn)%5;
                    unsigned counts[4]={a,b,c,d};
                    if(h<4){if(counts[h]==14)continue;++counts[h];}
                    else if(step-used==14)continue;
                    next[index208(counts[0],counts[1],counts[2],counts[3],h)]+=ways;
                }
            }
        }
        uint64_t *swap=current;current=next;next=swap;
    }
    uint64_t answer=current[index208(14,14,14,14,0)];
    printf("%" PRIu64 "\n",answer); free(current);free(next);return 0;
}
