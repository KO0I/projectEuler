/* Project Euler 69: the corrected algorithm is in support/c/euler_common.c. */
#include <stdio.h>
#include "euler_common.h"
int main(void) {
    long long answer = euler_solve(69);
    if (answer < 0 && 69 != 27) return 1;
    printf("%lld\n", answer);
    return 0;
}
