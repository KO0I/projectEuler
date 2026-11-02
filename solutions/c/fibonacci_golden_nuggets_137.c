/* Project Euler 137: Fibonacci Golden Nuggets
 *
 * Requiring x/(1-x-x^2) to be integral turns its discriminant condition into
 * a Pell equation.  Taking successive positive Pell solutions yields the
 * nuggets directly, with recurrence
 *
 *     A_1=2, A_2=15, A_n=7A_(n-1)-A_(n-2)+1.
 *
 * Advancing through the fifteenth term avoids floating-point roots and tests
 * no non-candidates.  The requested term fits uint64_t. */
#include "euler_extra.h"

int main(void) {
    uint64_t previous = 2, current = 15;
    for (unsigned index = 2; index < 15; ++index) {
        uint64_t next = 7 * current - previous + 1;
        previous = current;
        current = next;
    }
    printf("%" PRIu64 "\n", current);
    return 0;
}
