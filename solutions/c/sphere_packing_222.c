#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 222: sphere packing.
 *
 * Two tangent balls of radii a,b can put their centres on opposite sides of
 * the radius-50 pipe.  Their radial separation is (50-a)+(50-b), while their
 * full centre distance is a+b.  Pythagoras therefore gives the minimum axial
 * separation
 *
 *       sqrt((a+b)^2-(100-a-b)^2) = sqrt(200(a+b-50)).
 *
 * Choosing the ball order is consequently a shortest Hamiltonian path on 21
 * radii, with endpoint costs equal to the endpoint radii for the two end-cap
 * clearances.  Held-Karp subset DP finds the global optimum.  Float storage
 * keeps the 2^21*21 table practical; after selecting the route, its length is
 * recomputed in double precision before rounding to micrometres.
 */

enum { BALLS = 21 };

int main(void) {
    const uint32_t states = UINT32_C(1) << BALLS;
    const size_t entries = (size_t)states * BALLS;
    float *dp = malloc(entries * sizeof(*dp));
    if (dp == NULL) {
        return 1;
    }
    for (size_t i = 0; i < entries; ++i) {
        dp[i] = FLT_MAX;
    }

    double separation[BALLS][BALLS];
    for (int i = 0; i < BALLS; ++i) {
        for (int j = 0; j < BALLS; ++j) {
            const int a = 30 + i;
            const int b = 30 + j;
            separation[i][j] = sqrt(200.0 * (a + b - 50));
        }
        dp[((size_t)UINT32_C(1) << i) * BALLS + i] = (float)(30 + i);
    }

    for (uint32_t mask = 1; mask < states; ++mask) {
        uint32_t last_bits = mask;
        while (last_bits != 0) {
            const int last = __builtin_ctz(last_bits);
            last_bits &= last_bits - 1;
            const uint32_t previous_mask = mask ^ (UINT32_C(1) << last);
            if (previous_mask == 0) continue;
            float best = FLT_MAX;
            uint32_t previous_bits = previous_mask;
            while (previous_bits != 0) {
                const int previous = __builtin_ctz(previous_bits);
                previous_bits &= previous_bits - 1;
                const float candidate = dp[(size_t)previous_mask * BALLS + previous] +
                                        (float)separation[previous][last];
                if (candidate < best) best = candidate;
            }
            dp[(size_t)mask * BALLS + last] = best;
        }
    }

    const uint32_t full = states - 1;
    int last = 0;
    float best_total = FLT_MAX;
    for (int i = 0; i < BALLS; ++i) {
        const float total = dp[(size_t)full * BALLS + i] + (float)(30 + i);
        if (total < best_total) {
            best_total = total;
            last = i;
        }
    }

    /* Reconstruct by re-evaluating the predecessor minimum encoded by each
       DP cell.  No separate 44-million-byte parent table is necessary. */
    int reverse_order[BALLS];
    uint32_t mask = full;
    for (int position = BALLS - 1; position >= 0; --position) {
        reverse_order[position] = last;
        const uint32_t previous_mask = mask ^ (UINT32_C(1) << last);
        if (previous_mask == 0) break;
        int best_previous = -1;
        float best = FLT_MAX;
        uint32_t bits = previous_mask;
        while (bits != 0) {
            const int previous = __builtin_ctz(bits);
            bits &= bits - 1;
            const float candidate = dp[(size_t)previous_mask * BALLS + previous] +
                                    (float)separation[previous][last];
            if (candidate < best) {
                best = candidate;
                best_previous = previous;
            }
        }
        mask = previous_mask;
        last = best_previous;
    }

    double length_mm = 30 + reverse_order[0] + 30 + reverse_order[BALLS - 1];
    for (int i = 1; i < BALLS; ++i) {
        length_mm += separation[reverse_order[i - 1]][reverse_order[i]];
    }
    printf("%lld\n", (long long)llround(length_mm * 1000.0));
    free(dp);
    return 0;
}
