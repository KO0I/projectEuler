/* Project Euler 197: https://projecteuler.net/problem=197

   Direct iteration is the intended numerical process.  The recurrence rapidly
   settles into a two-cycle, so u_n+u_(n+1) stabilizes long before n=10^12.
   Iterating 2000 times is deliberately excessive and printing nine places
   matches the requested rounding. */
#include "euler_extra.h"
static double next197(double value) {
    return floor(pow(2.0, 30.403243784 - value * value)) * 1e-9;
}
int main(void) {
    double current = -1.0, next = 0;
    for (unsigned i = 0; i < 2000; ++i) { next = next197(current); current = next; }
    printf("%.9f\n", current + next197(current));
    return 0;
}
