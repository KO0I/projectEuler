#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 253: Tidying Up A
 *
 * The direct process is awkward: after each randomly chosen piece is added,
 * we would need to remember which empty intervals touch an end of the board.
 * Reverse the random permutation instead.  Starting with the complete
 * caterpillar, delete its pieces in reverse pickup order.  At every time k,
 * the pieces left in the reversed process are exactly the pieces already
 * placed after 40-k forward steps.  Consequently both processes encounter
 * the same segment counts, only in the opposite order, and have the same M.
 *
 * A reversed state needs only the sorted lengths of its connected segments.
 * It is therefore an integer partition of at most 40 rather than a detailed
 * 40-cell bitmap.  Deleting a piece from a segment of length L has three
 * effects:
 *
 *   L = 1: the segment disappears;
 *   endpoint: one segment of length L-1 remains (two endpoint choices);
 *   interior: lengths x and L-1-x remain (one or two symmetric choices).
 *
 * Equal-length segments and mirror-image cuts lead to identical child
 * partitions, so their multiplicities are combined.  The transition weights
 * still sum to the number of pieces remaining, making their weighted average
 * the exact conditional expectation.
 *
 * E(state,best) memoizes the expected final maximum, where best is the largest
 * number of segments seen so far.  Segment lengths fit in bytes (at most 40),
 * and no state has more than 20 nonempty segments, so three 64-bit words pack
 * the complete canonical state without collision.  Floating-point arithmetic
 * is used only for the final probability averages; all combinatorial weights
 * and state transformations are exact integers.
 */

enum { PIECES = 40, MAX_SEGMENTS = 20, HASH_SIZE = 1 << 20 };

typedef struct {
    uint64_t key[3];
    double expectation;
    unsigned char best;
    unsigned char used;
} Entry;

static Entry *memo;

static void sort_lengths(unsigned char *lengths, int count) {
    for (int i = 1; i < count; ++i) {
        unsigned char value = lengths[i];
        int j = i;
        while (j > 0 && lengths[j - 1] > value) {
            lengths[j] = lengths[j - 1];
            --j;
        }
        lengths[j] = value;
    }
}

static void pack_state(const unsigned char *lengths, int count,
                       uint64_t key[3]) {
    key[0] = key[1] = key[2] = 0;
    for (int i = 0; i < count; ++i) {
        key[i / 8] |= (uint64_t)lengths[i] << (8 * (i % 8));
    }
}

static int unpack_state(const uint64_t key[3], unsigned char *lengths) {
    int count = 0;
    for (int i = 0; i < MAX_SEGMENTS; ++i) {
        unsigned char value =
            (unsigned char)(key[i / 8] >> (8 * (i % 8)));
        if (value == 0) {
            break;
        }
        lengths[count++] = value;
    }
    return count;
}

static uint64_t state_hash(const uint64_t key[3], unsigned char best) {
    uint64_t hash = key[0] ^ ((uint64_t)best * UINT64_C(0x9e3779b97f4a7c15));
    hash ^= key[1] + UINT64_C(0x9e3779b97f4a7c15) + (hash << 6) + (hash >> 2);
    hash ^= key[2] + UINT64_C(0x9e3779b97f4a7c15) + (hash << 6) + (hash >> 2);
    return hash;
}

static int same_key(const uint64_t a[3], const uint64_t b[3]) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

static double expected_maximum(const uint64_t key[3], unsigned char best) {
    unsigned char state[MAX_SEGMENTS + 1];
    int segment_count = unpack_state(key, state);
    if (segment_count == 0) {
        return (double)best;
    }

    size_t slot = (size_t)(state_hash(key, best) & (HASH_SIZE - 1));
    while (memo[slot].used &&
           (memo[slot].best != best || !same_key(memo[slot].key, key))) {
        slot = (slot + 1) & (HASH_SIZE - 1);
    }
    if (memo[slot].used) {
        return memo[slot].expectation;
    }

    int remaining = 0;
    for (int i = 0; i < segment_count; ++i) {
        remaining += state[i];
    }

    double weighted_sum = 0.0;
    for (int first = 0; first < segment_count;) {
        int after_equal = first + 1;
        while (after_equal < segment_count &&
               state[after_equal] == state[first]) {
            ++after_equal;
        }
        int copies = after_equal - first;
        int length = state[first];

        /* Remove one representative segment.  Multiplying every resulting
         * cut by copies accounts for choosing any equal segment. */
        unsigned char base[MAX_SEGMENTS + 1];
        int base_count = 0;
        for (int i = 0; i < segment_count; ++i) {
            if (i != first) {
                base[base_count++] = state[i];
            }
        }

        if (length == 1) {
            uint64_t child_key[3];
            pack_state(base, base_count, child_key);
            weighted_sum += copies * expected_maximum(child_key, best);
        } else {
            /* Either endpoint gives the same partition, hence weight 2. */
            unsigned char child[MAX_SEGMENTS + 1];
            for (int i = 0; i < base_count; ++i) {
                child[i] = base[i];
            }
            child[base_count] = (unsigned char)(length - 1);
            sort_lengths(child, base_count + 1);
            uint64_t child_key[3];
            pack_state(child, base_count + 1, child_key);
            unsigned char child_best = best;
            if (base_count + 1 > child_best) {
                child_best = (unsigned char)(base_count + 1);
            }
            weighted_sum += 2.0 * copies *
                            expected_maximum(child_key, child_best);

            /* Interior deletion splits the segment.  Only enumerate the
             * smaller half; unequal mirror cuts have multiplicity two. */
            int rest = length - 1;
            for (int left = 1; left <= rest / 2; ++left) {
                int right = rest - left;
                for (int i = 0; i < base_count; ++i) {
                    child[i] = base[i];
                }
                child[base_count] = (unsigned char)left;
                child[base_count + 1] = (unsigned char)right;
                sort_lengths(child, base_count + 2);
                pack_state(child, base_count + 2, child_key);
                child_best = best;
                if (base_count + 2 > child_best) {
                    child_best = (unsigned char)(base_count + 2);
                }
                int mirror_weight = left == right ? 1 : 2;
                weighted_sum += (double)(copies * mirror_weight) *
                                expected_maximum(child_key, child_best);
            }
        }
        first = after_equal;
    }

    double result = weighted_sum / remaining;
    memo[slot].key[0] = key[0];
    memo[slot].key[1] = key[1];
    memo[slot].key[2] = key[2];
    memo[slot].best = best;
    memo[slot].expectation = result;
    memo[slot].used = 1;
    return result;
}

int main(void) {
    memo = calloc(HASH_SIZE, sizeof(*memo));
    if (memo == NULL) {
        return 1;
    }

    unsigned char initial[] = {PIECES};
    uint64_t initial_key[3];
    pack_state(initial, 1, initial_key);
    double answer = expected_maximum(initial_key, 1);
    printf("%.6f\n", answer);

    free(memo);
    return 0;
}
