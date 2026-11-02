#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 * Project Euler 230: Fibonacci Words.
 *
 * Store only term lengths.  Term i is term i-2 followed by term i-1, so a
 * requested position either stays in i-2 or has len[i-2] removed and moves to
 * i-1.  Repeating this descent reaches one of the two 100-digit seed strings
 * without ever constructing an exponentially long word.
 *
 * Length addition is saturated at UINT64_MAX; all requested positions fit in
 * uint64_t, and values beyond that threshold are needed only for comparison.
 */

static const char A[] =
    "14159265358979323846264338327950288419716939937510"
    "58209749445923078164062862089986280348253421170679";
static const char B[] =
    "82148086513282306647093844609550582231725359408128"
    "48111745028410270193852110555964462294895493038196";

static int digit_at(uint64_t position) {
    uint64_t length[100] = {100, 100};
    int term = 1;
    while (length[term] < position) {
        ++term;
        if (UINT64_MAX - length[term - 1] < length[term - 2]) {
            length[term] = UINT64_MAX;
        } else {
            length[term] = length[term - 2] + length[term - 1];
        }
    }
    while (term >= 2) {
        if (position <= length[term - 2]) {
            term -= 2;
        } else {
            position -= length[term - 2];
            term -= 1;
        }
    }
    return (term == 0 ? A[position - 1] : B[position - 1]) - '0';
}

int main(void) {
    (void)strlen(A); /* Compile-time strings are intentionally visibly 100 digits. */
    uint64_t sum = 0;
    uint64_t power7 = 1;
    uint64_t place10 = 1;
    for (uint64_t n = 0; n <= 17; ++n) {
        const uint64_t position = (127 + 19 * n) * power7;
        sum += (uint64_t)digit_at(position) * place10;
        power7 *= 7;
        place10 *= 10;
    }
    printf("%llu\n", (unsigned long long)sum);
    return 0;
}
