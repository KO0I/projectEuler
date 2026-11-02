/* Project Euler 138: Special Isosceles Triangles
 *
 * Substituting h=b+-1 into L^2=h^2+(b/2)^2 reduces the integer-triangle
 * condition to a Pell equation.  Ordering its positive solutions gives the
 * linear recurrence
 *
 *     L_0=1, L_1=17, L_(n+1)=18L_n-L_(n-1).
 *
 * The degenerate seed 1 is used only to advance the recurrence; the twelve
 * requested lengths begin at 17.  Their sum remains below 2^64. */
#include "euler_extra.h"

int main(void) {
    uint64_t previous = 1, current = 17, sum = current;
    for (unsigned index = 1; index < 12; ++index) {
        uint64_t next = 18 * current - previous;
        previous = current;
        current = next;
        sum += current;
    }
    printf("%" PRIu64 "\n", sum);
    return 0;
}
