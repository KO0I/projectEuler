#include <stdio.h>

/*
 * Project Euler 232: The Race
 *
 * Let P[a][b] be player 2's eventual winning probability immediately before
 * player 1 moves, and Q[a][b] the probability before player 2 moves.  Scores
 * a and b below 100 are the only non-terminal states.
 *
 * Player 1's fair toss gives
 *
 *     P[a][b] = (Q[a + 1][b] + Q[a][b]) / 2.
 *
 * For player 2's choice T, put p=2^-T and g=2^(T-1).  A success moves to
 * P[a][b+g], while failure returns to P[a][b].  Substituting the equation
 * above creates Q[a][b] on both sides (both players can score nothing).
 * Solving that one-state loop algebraically gives
 *
 *   Q_T = (p P[a][b+g] + (1-p) Q[a+1][b]/2)
 *         / (1 - (1-p)/2).
 *
 * Thus a simple descending table fill replaces an infinite stochastic loop.
 * T=1..8 suffices: T=8 already awards 128 points, and larger T has the same
 * winning successor but a strictly smaller success probability.
 */

int main(void) {
    enum { TARGET = 100, MAX_GAIN = 128 };
    double p1_turn[TARGET][TARGET + MAX_GAIN];
    double p2_turn[TARGET + 1][TARGET + MAX_GAIN];

    /* Scores at or above 100 are wins for player 2, regardless of whose turn
       the notation says it is.  Zero initialization handles all live cells. */
    for (int a = 0; a < TARGET; ++a) {
        for (int b = 0; b < TARGET + MAX_GAIN; ++b) {
            p1_turn[a][b] = b >= TARGET ? 1.0 : 0.0;
            p2_turn[a][b] = b >= TARGET ? 1.0 : 0.0;
        }
    }
    /* Row a=100 means player 1 has already won. */
    for (int b = 0; b < TARGET + MAX_GAIN; ++b)
        p2_turn[TARGET][b] = 0.0;

    /* Every dependency has either a larger a or a larger b, so descending
       scores are a valid topological order for the dynamic program. */
    for (int a = TARGET - 1; a >= 0; --a) {
        for (int b = TARGET - 1; b >= 0; --b) {
            const double after_p1_head = p2_turn[a + 1][b];
            double best = 0.0;
            int gain = 1;
            double success_probability = 0.5;

            for (int tosses = 1; tosses <= 8; ++tosses) {
                const double success = p1_turn[a][b + gain];
                const double failure = 1.0 - success_probability;
                const double value =
                    (success_probability * success
                     + 0.5 * failure * after_p1_head)
                    / (1.0 - 0.5 * failure);
                if (value > best)
                    best = value;

                gain *= 2;
                success_probability *= 0.5;
            }
            p2_turn[a][b] = best;
            p1_turn[a][b] = 0.5 * (after_p1_head + best);
        }
    }

    printf("%.8f\n", p1_turn[0][0]);
    return 0;
}
