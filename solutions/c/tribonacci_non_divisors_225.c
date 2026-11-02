#include <stdio.h>

/*
 * Project Euler 225: Tribonacci non-divisors.
 *
 * Work modulo each odd candidate m.  A state is the last three residues and
 * the transition (a,b,c)->(b,c,a+b+c) is invertible: the previous residue is
 * new_c-new_a-new_b.  Consequently the finite sequence is purely periodic
 * and must return to (1,1,1).  If residue zero appears first, m divides a
 * Tribonacci number; if the initial state returns first, it never will.
 * Even m are irrelevant because the parity sequence contains zero.
 */

static int never_divides(int modulus) {
    int a = 1;
    int b = 1;
    int c = 1;
    do {
        const int next = (a + b + c) % modulus;
        a = b;
        b = c;
        c = next;
        if (c == 0) return 0;
    } while (a != 1 || b != 1 || c != 1);
    return 1;
}

int main(void) {
    int found = 0;
    for (int candidate = 1;; candidate += 2) {
        if (never_divides(candidate) && ++found == 124) {
            printf("%d\n", candidate);
            return 0;
        }
    }
}
