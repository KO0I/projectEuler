#include <math.h>
#include <stdio.h>
#include <string.h>

/*
 * Project Euler 213: Flea Circus.
 *
 * Fleas move independently.  For one starting square, a 50-step probability
 * DP distributes its flea over the board, dividing by the actual degree (2,
 * 3, or 4) of each square.  If that flea reaches target t with probability p,
 * it misses t with probability 1-p.  Multiplying these miss probabilities
 * over all 900 starting fleas gives the probability that t is empty.
 *
 * Products of 900 values are accumulated as sum(log1p(-p)); this is both more
 * numerically stable and less prone to underflow than repeated multiplication.
 * Linearity of expectation then allows the 900 empty-square probabilities to
 * be summed even though the empty events themselves are not independent.
 */

enum { SIDE = 30, SQUARES = SIDE * SIDE, STEPS = 50 };

static int degree(int row, int column) {
    return 4 - (row == 0) - (row == SIDE - 1) - (column == 0) - (column == SIDE - 1);
}

int main(void) {
    double log_empty[SQUARES] = {0.0};
    double current[SQUARES];
    double next[SQUARES];

    for (int start = 0; start < SQUARES; ++start) {
        memset(current, 0, sizeof(current));
        current[start] = 1.0;
        for (int step = 0; step < STEPS; ++step) {
            memset(next, 0, sizeof(next));
            for (int row = 0; row < SIDE; ++row) {
                for (int column = 0; column < SIDE; ++column) {
                    const int index = row * SIDE + column;
                    const double share = current[index] / degree(row, column);
                    if (row > 0) next[index - SIDE] += share;
                    if (row + 1 < SIDE) next[index + SIDE] += share;
                    if (column > 0) next[index - 1] += share;
                    if (column + 1 < SIDE) next[index + 1] += share;
                }
            }
            memcpy(current, next, sizeof(current));
        }
        for (int target = 0; target < SQUARES; ++target) {
            log_empty[target] += log1p(-current[target]);
        }
    }

    double expectation = 0.0;
    for (int target = 0; target < SQUARES; ++target) {
        expectation += exp(log_empty[target]);
    }
    printf("%.6f\n", expectation);
    return 0;
}
