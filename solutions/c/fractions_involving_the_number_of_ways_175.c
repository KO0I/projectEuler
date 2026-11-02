/* Project Euler 175: Fractions Involving the Number of Ways
 *
 * The ratio of adjacent Stern-Brocot/Calkin-Wilf counts is decoded by the
 * subtractive Euclidean algorithm.  Instead of subtracting one copy at a time,
 * `(larger-1)/smaller` records an entire run while preserving a positive
 * remainder.  These quotients are precisely the run lengths in the requested
 * shortened binary expansion.
 *
 * Euclid discovers runs from the leaf back toward the root, so they are printed
 * in reverse order after appending the root run of length one.  uint64_t easily
 * covers both the given reduced fraction and every intermediate subtraction. */
#include "euler_extra.h"

int main(void) {
    uint64_t numerator = 123456789;
    uint64_t denominator = 987654321;
    uint64_t runs[32];
    unsigned count = 0;
    while (numerator != denominator) {
        if (numerator < denominator) {
            uint64_t quotient = (denominator - 1) / numerator;
            runs[count++] = quotient;
            denominator -= quotient * numerator;
        } else {
            uint64_t quotient = (numerator - 1) / denominator;
            runs[count++] = quotient;
            numerator -= quotient * denominator;
        }
    }
    runs[count++] = 1;
    for (unsigned i = count; i-- > 0;)
        printf("%" PRIu64 "%c", runs[i], i ? ',' : '\n');
    return 0;
}
