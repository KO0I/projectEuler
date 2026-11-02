#include <math.h>
#include <stdio.h>

/*
 * Project Euler 227: The Chase.
 *
 * Rotational symmetry reduces the configuration to the clockwise separation
 * k of the two dice.  Each die moves -1,0,+1 with probabilities 1/6,4/6,1/6,
 * so their relative displacement -2..2 has probabilities
 * 1/36,2/9,1/2,2/9,1/36.  With E[0]=0, first-step analysis gives
 *
 *       E[k] - sum_j P(k->j)E[j] = 1
 *
 * for the 99 transient separations.  Dense Gaussian elimination with partial
 * pivoting solves this deterministic expectation system; the opposite-seat
 * starting state is E[50].
 */

enum { PLAYERS = 100, UNKNOWNS = PLAYERS - 1 };

int main(void) {
    double matrix[UNKNOWNS][UNKNOWNS + 1] = {{0.0}};
    static const int shift[5] = {-2, -1, 0, 1, 2};
    static const double probability[5] = {1.0/36.0, 2.0/9.0, 0.5, 2.0/9.0, 1.0/36.0};
    for (int k = 1; k < PLAYERS; ++k) {
        const int row = k - 1;
        matrix[row][row] = 1.0;
        matrix[row][UNKNOWNS] = 1.0;
        for (int i = 0; i < 5; ++i) {
            int next = (k + shift[i]) % PLAYERS;
            if (next < 0) next += PLAYERS;
            if (next != 0) matrix[row][next - 1] -= probability[i];
        }
    }

    for (int column = 0; column < UNKNOWNS; ++column) {
        int pivot = column;
        for (int row = column + 1; row < UNKNOWNS; ++row) {
            if (fabs(matrix[row][column]) > fabs(matrix[pivot][column])) pivot = row;
        }
        if (pivot != column) {
            for (int j = column; j <= UNKNOWNS; ++j) {
                const double temporary = matrix[column][j];
                matrix[column][j] = matrix[pivot][j];
                matrix[pivot][j] = temporary;
            }
        }
        const double divisor = matrix[column][column];
        for (int j = column; j <= UNKNOWNS; ++j) matrix[column][j] /= divisor;
        for (int row = 0; row < UNKNOWNS; ++row) {
            if (row == column) continue;
            const double factor = matrix[row][column];
            if (factor == 0.0) continue;
            for (int j = column; j <= UNKNOWNS; ++j) {
                matrix[row][j] -= factor * matrix[column][j];
            }
        }
    }
    printf("%.6f\n", matrix[49][UNKNOWNS]);
    return 0;
}
