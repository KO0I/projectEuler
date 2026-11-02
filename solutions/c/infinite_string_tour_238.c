#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 238: Infinite String Tour
 *
 * The modular squaring sequence eventually returns to its initial seed; for
 * these parameters that happens before any transient, so the concatenated
 * digit string is a repeated finite block.  Let its length be L, digit sum S,
 * and P_i the digit prefix sum through position i in one block.
 *
 * A substring beginning at z=b+1 has sum k exactly when a later infinite
 * prefix equals P_b+k.  Infinite prefix sums are q*S+P_i, hence such an end
 * exists iff
 *
 *       (P_b + k) mod S
 *
 * is one of the one-period prefix residues.  This test also proves
 * p(k+S)=p(k), and an attainable target's earliest start lies in the first
 * digit period.  We store the L starting residues and a bitset for membership,
 * then scan starts in order for each of the S target residues.  Prefix residues
 * occupy a substantial fraction of [0,S), so the average scan is short even
 * though the conservative worst-case bound is L.
 */

enum { MODULUS = 20300713, INITIAL = 14025256 };
static const uint64_t TARGET = UINT64_C(2000000000000000);

static void append_number(uint32_t value, uint32_t **prefixes, size_t *length,
                          size_t *capacity, uint32_t *digit_sum) {
    uint32_t divisor = 1;
    while (divisor <= value / 10)
        divisor *= 10;

    do {
        const uint32_t digit = value / divisor;
        value %= divisor;
        divisor /= 10;

        if (*length == *capacity) {
            *capacity *= 2;
            uint32_t *grown = realloc(*prefixes,
                                      *capacity * sizeof(**prefixes));
            if (grown == NULL) {
                free(*prefixes);
                exit(1);
            }
            *prefixes = grown;
        }
        /* Store P_b, the sum before this digit, because it corresponds to
           substring start z=b+1. */
        (*prefixes)[(*length)++] = *digit_sum;
        *digit_sum += digit;
    } while (divisor != 0);
}

int main(void) {
    size_t capacity = 1U << 20;
    size_t length = 0;
    uint32_t *prefixes = malloc(capacity * sizeof(*prefixes));
    if (prefixes == NULL)
        return 1;

    uint32_t digit_sum = 0;
    uint32_t state = INITIAL;
    do {
        append_number(state, &prefixes, &length, &capacity, &digit_sum);
        state = (uint32_t)((uint64_t)state * state % MODULUS);
    } while (state != INITIAL);

    /* A bitset cuts the residue table from roughly 81 MB to 10 MB.  P_L=S
       has residue zero, which P_0 already supplies. */
    const size_t bit_bytes = ((size_t)digit_sum + 7) / 8;
    unsigned char *present = calloc(bit_bytes, 1);
    if (present == NULL) {
        free(prefixes);
        return 1;
    }
    for (size_t i = 0; i < length; ++i) {
        const uint32_t residue = prefixes[i];
        present[residue >> 3] |= (unsigned char)(1U << (residue & 7U));
    }

    const uint64_t complete_periods = TARGET / digit_sum;
    const uint32_t remainder = (uint32_t)(TARGET % digit_sum);
    uint64_t period_sum = 0;
    uint64_t remainder_sum = 0;

    for (uint32_t k = 1; k <= digit_sum; ++k) {
        const uint32_t target_residue = k == digit_sum ? 0 : k;
        size_t start = 0;
        for (; start < length; ++start) {
            uint32_t wanted = prefixes[start] + target_residue;
            if (wanted >= digit_sum)
                wanted -= digit_sum;
            if (present[wanted >> 3]
                & (unsigned char)(1U << (wanted & 7U)))
                break;
        }
        const uint64_t position = (uint64_t)start + 1;
        period_sum += position;
        if (k <= remainder)
            remainder_sum += position;
    }

    printf("%llu\n",
           (unsigned long long)(complete_periods * period_sum + remainder_sum));
    free(present);
    free(prefixes);
    return 0;
}
