/* Project Euler 36: https://projecteuler.net/problem=36
 * Reverse the ordinary, unpadded representation independently in bases
 * ten and two. Binary palindromes other than zero are necessarily odd. */
#include "euler_extra.h"

static bool palindrome(unsigned n, unsigned base) {
    unsigned original = n, reversed = 0;
    do { reversed = base * reversed + n % base; n /= base; } while (n);
    return reversed == original;
}

int main(void) {
    unsigned sum = 0;
    for (unsigned n = 1; n < 1000000; n += 2)
        if (palindrome(n, 10) && palindrome(n, 2)) sum += n;
    printf("%u\n", sum);
    return 0;
}
