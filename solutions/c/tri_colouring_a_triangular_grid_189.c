#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 189: tri-colouring a triangular grid.
 *
 * Row r contains r upward-pointing triangles.  Between adjacent upward
 * triangles is one downward triangle, whose three neighbours are precisely
 * the two current upward triangles and upward triangle j of the preceding
 * row.  Once the colours of the upward triangles in two consecutive rows are
 * known, every intervening downward triangle has
 *
 *     3 - number_of_distinct(neighbour colours)
 *
 * choices.  The choices for different downward triangles are independent.
 * Thus a DP state needs only the base-3 colour tuple of the upward triangles
 * in the last row; no individual downward colour survives into the frontier.
 * There are at most 3^8 = 6561 states, and the complete transition work is
 * about 3^7 * 3^8 small integer operations.
 */

enum { SIDE = 8, MAX_STATES = 6561 };

static int power3(int exponent) {
    int result = 1;
    while (exponent-- > 0) {
        result *= 3;
    }
    return result;
}

static void decode_colours(int state, int length, unsigned char *colours) {
    for (int i = 0; i < length; ++i) {
        colours[i] = (unsigned char)(state % 3);
        state /= 3;
    }
}

int main(void) {
    uint64_t *previous = calloc(MAX_STATES, sizeof(*previous));
    uint64_t *current = calloc(MAX_STATES, sizeof(*current));
    unsigned char (*decoded)[SIDE] = malloc(MAX_STATES * sizeof(*decoded));
    if (previous == NULL || current == NULL || decoded == NULL) {
        free(decoded);
        free(current);
        free(previous);
        return 1;
    }

    for (int state = 0; state < MAX_STATES; ++state) {
        decode_colours(state, SIDE, decoded[state]);
    }

    /* The top row is one upward triangle and may have any of three colours. */
    previous[0] = previous[1] = previous[2] = 1;
    for (int row = 2; row <= SIDE; ++row) {
        const int previous_states = power3(row - 1);
        const int current_states = power3(row);
        for (int state = 0; state < current_states; ++state) {
            current[state] = 0;
        }

        for (int before = 0; before < previous_states; ++before) {
            if (previous[before] == 0) {
                continue;
            }
            for (int after = 0; after < current_states; ++after) {
                uint64_t multiplicity = 1;
                for (int j = 0; j < row - 1; ++j) {
                    const int a = decoded[before][j];
                    const int b = decoded[after][j];
                    const int c = decoded[after][j + 1];
                    const int distinct = 1 + (b != a) + (c != a && c != b);
                    multiplicity *= (uint64_t)(3 - distinct);
                    if (multiplicity == 0) {
                        break;
                    }
                }
                current[after] += previous[before] * multiplicity;
            }
        }

        uint64_t *swap = previous;
        previous = current;
        current = swap;
    }

    uint64_t total = 0;
    for (int state = 0; state < power3(SIDE); ++state) {
        total += previous[state];
    }
    printf("%llu\n", (unsigned long long)total);
    free(decoded);
    free(current);
    free(previous);
    return 0;
}
