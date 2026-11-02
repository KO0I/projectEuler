#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 254: Sums of Digit Factorials
 *
 * Searching n is hopeless because some g(i) contain billions of digits.  We
 * search k=f(n) instead.  For a fixed k, the smallest n whose digit factorials
 * sum to k is obtained from the factorial-number-system expansion
 *
 *   k = c9*9! + c8*8! + ... + c1*1!,    0 <= cd <= d (d < 9).
 *
 * The corresponding n consists of c1 ones, then c2 twos, and so on.  Sorting
 * gives the smallest ordering.  The coefficient limits are forced by carrying
 * (d+1) copies of d! into one (d+1)!; every such carry shortens n.  Zeros are
 * unnecessary because 0!=1!=1 and a one is never a leading zero.
 *
 * Thus g(i) is the best factorial representation among k with digit_sum(k)=i.
 * Write k=q*9!+r.  The remainder r uniquely determines c1..c8 and q is c9.
 * Let k_min be the smallest ordinary decimal integer with digit sum i.  No
 * candidate has q<floor(k_min/9!).  The representation of k_min itself has at
 * most 1+2+...+8=36 lower digits, so a q more than 36 above that lower bound
 * cannot possibly beat it on total length.  Only 37 q values are relevant.
 *
 * Remainders are grouped by r mod 9: digit_sum(k) == k == r (mod 9), since
 * 9! is divisible by 9.  Within each group they are sorted by the decimal n
 * they induce (fewest digits, then most low digits).  For a fixed q, the first
 * remainder with digit_sum(q*9!+r)=i is therefore the best one for that q.
 * We compare those candidates and accumulate sg(i) without ever materializing
 * the enormous decimal strings.
 */

enum { LIMIT = 150, FACT9 = 362880, BUCKET_SIZE = FACT9 / 9 };

typedef struct {
    uint32_t remainder;
    uint16_t digit_sum;
    unsigned char length;
    unsigned char count[8];
} Remainder;

typedef struct {
    uint64_t nines;
    uint16_t lower_digit_sum;
    unsigned char lower_length;
    unsigned char count[8];
} Candidate;

static int compare_remainders(const void *left_pointer,
                              const void *right_pointer) {
    const Remainder *left = left_pointer;
    const Remainder *right = right_pointer;
    if (left->length != right->length) {
        return left->length < right->length ? -1 : 1;
    }
    /* With equal total length, the sorted decimal string with more copies of
     * the first differing low digit is lexicographically smaller. */
    for (int d = 0; d < 8; ++d) {
        if (left->count[d] != right->count[d]) {
            return left->count[d] > right->count[d] ? -1 : 1;
        }
    }
    return 0;
}

static unsigned digit_sum_u64(uint64_t value) {
    unsigned sum = 0;
    while (value != 0) {
        sum += (unsigned)(value % 10);
        value /= 10;
    }
    return sum;
}

static uint64_t smallest_with_digit_sum(unsigned sum) {
    /* A shortest decimal number uses as many 9s as possible; putting the
     * non-9 remainder first makes that shortest number lexicographically least. */
    unsigned nines = sum / 9;
    unsigned first = sum % 9;
    uint64_t value = first;
    if (first == 0) {
        value = 9;
        --nines;
    }
    while (nines-- != 0) {
        value = value * 10 + 9;
    }
    return value;
}

static Remainder decode_remainder(uint32_t remainder) {
    static const uint32_t factorial[9] =
        {1, 1, 2, 6, 24, 120, 720, 5040, 40320};
    Remainder decoded = {0};
    decoded.remainder = remainder;
    uint32_t value = remainder;
    for (int d = 8; d >= 1; --d) {
        unsigned char count = (unsigned char)(value / factorial[d]);
        value %= factorial[d];
        decoded.count[d - 1] = count;
        decoded.length = (unsigned char)(decoded.length + count);
        decoded.digit_sum = (uint16_t)(decoded.digit_sum + d * count);
    }
    return decoded;
}

static Candidate make_candidate(uint64_t q, const Remainder *remainder) {
    Candidate candidate;
    candidate.nines = q;
    candidate.lower_digit_sum = remainder->digit_sum;
    candidate.lower_length = remainder->length;
    for (int i = 0; i < 8; ++i) {
        candidate.count[i] = remainder->count[i];
    }
    return candidate;
}

static int candidate_is_better(const Candidate *candidate,
                               const Candidate *best) {
    uint64_t candidate_length = candidate->nines + candidate->lower_length;
    uint64_t best_length = best->nines + best->lower_length;
    if (candidate_length != best_length) {
        return candidate_length < best_length;
    }
    for (int d = 0; d < 8; ++d) {
        if (candidate->count[d] != best->count[d]) {
            return candidate->count[d] > best->count[d];
        }
    }
    return candidate->nines > best->nines;
}

int main(void) {
    Remainder *buckets[9];
    size_t used[9] = {0};
    for (int residue = 0; residue < 9; ++residue) {
        buckets[residue] = malloc(BUCKET_SIZE * sizeof(*buckets[residue]));
        if (buckets[residue] == NULL) {
            for (int previous = 0; previous < residue; ++previous) {
                free(buckets[previous]);
            }
            return 1;
        }
    }

    for (uint32_t r = 0; r < FACT9; ++r) {
        int residue = (int)(r % 9);
        buckets[residue][used[residue]++] = decode_remainder(r);
    }
    for (int residue = 0; residue < 9; ++residue) {
        qsort(buckets[residue], used[residue], sizeof(*buckets[residue]),
              compare_remainders);
    }

    uint64_t total = 0;
    for (unsigned target = 1; target <= LIMIT; ++target) {
        uint64_t k_min = smallest_with_digit_sum(target);
        uint64_t q_min = k_min / FACT9;
        uint32_t r_min = (uint32_t)(k_min % FACT9);
        Remainder initial = decode_remainder(r_min);
        Candidate best = make_candidate(q_min, &initial);
        int residue = (int)(target % 9);

        for (uint64_t q = q_min; q <= q_min + 36; ++q) {
            /* Once q alone reaches the best known digit count, even a zero
             * remainder cannot produce a shorter n. */
            if (q >= best.nines + best.lower_length) {
                break;
            }
            uint64_t base = q * (uint64_t)FACT9;
            for (size_t j = 0; j < used[residue]; ++j) {
                const Remainder *r = &buckets[residue][j];
                if (digit_sum_u64(base + r->remainder) == target) {
                    Candidate candidate = make_candidate(q, r);
                    if (candidate_is_better(&candidate, &best)) {
                        best = candidate;
                    }
                    /* The bucket ordering proves every later remainder is no
                     * better for this fixed q. */
                    break;
                }
            }
        }
        total += best.lower_digit_sum + 9 * best.nines;
    }

    printf("%llu\n", (unsigned long long)total);
    for (int residue = 0; residue < 9; ++residue) {
        free(buckets[residue]);
    }
    return 0;
}
