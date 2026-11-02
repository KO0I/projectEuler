/* Project Euler 5: the corrected algorithm is in support/c/euler_common.c. */
#include <stdio.h>
#include "euler_common.h"
int main(void) {
    long long answer = euler_solve(5);
    if (answer < 0 && 5 != 27) return 1;
    printf("%lld\n", answer);
    return 0;
}
