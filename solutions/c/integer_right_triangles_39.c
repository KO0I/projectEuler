/* Project Euler 39: the corrected algorithm is in support/c/euler_common.c. */
#include <stdio.h>
#include "euler_common.h"
int main(void) {
    long long answer = euler_solve(39);
    if (answer < 0 && 39 != 27) return 1;
    printf("%lld\n", answer);
    return 0;
}
