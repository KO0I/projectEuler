#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 237: Tours on a 4 x N Playing Board
 *
 * Sweeping one column at a time, a partial Hamiltonian path is characterized
 * by how its open ends are paired across the four-cell frontier.  Enumerating
 * those few connectivity states and eliminating the auxiliary states gives
 *
 *   T(n) = 2T(n-1) + 2T(n-2) - 2T(n-3) + T(n-4),
 *   T(1..4) = 1, 1, 4, 8.
 *
 * The requested n=10^12 rules out linear iteration.  We encode the recurrence
 * as a 4x4 transition and use binary exponentiation, reducing every multiply
 * modulo 10^8.  Entries are below 10^8, so a sum of four products remains
 * safely inside uint64_t.  The negative coefficient is stored as MOD-2.
 */

enum { SIZE = 4 };
static const uint64_t MODULUS = UINT64_C(100000000);

typedef struct {
    uint64_t cell[SIZE][SIZE];
} Matrix;

static Matrix multiply(Matrix left, Matrix right) {
    Matrix result = {{{0}}};
    for (int row = 0; row < SIZE; ++row) {
        for (int column = 0; column < SIZE; ++column) {
            uint64_t value = 0;
            for (int k = 0; k < SIZE; ++k)
                value += left.cell[row][k] * right.cell[k][column];
            result.cell[row][column] = value % MODULUS;
        }
    }
    return result;
}

static Matrix power(Matrix base, uint64_t exponent) {
    Matrix result = {{{0}}};
    for (int i = 0; i < SIZE; ++i)
        result.cell[i][i] = 1;
    while (exponent != 0) {
        if (exponent & 1U)
            result = multiply(result, base);
        base = multiply(base, base);
        exponent >>= 1;
    }
    return result;
}

int main(void) {
    const uint64_t n = UINT64_C(1000000000000);
    Matrix transition = {{{0}}};
    transition.cell[0][1] = 1;
    transition.cell[1][2] = 1;
    transition.cell[2][3] = 1;
    transition.cell[3][0] = 1;
    transition.cell[3][1] = MODULUS - 2;
    transition.cell[3][2] = 2;
    transition.cell[3][3] = 2;

    const Matrix jump = power(transition, n - 4);
    const uint64_t initial[SIZE] = {1, 1, 4, 8};
    uint64_t answer = 0;
    for (int i = 0; i < SIZE; ++i)
        answer = (answer + jump.cell[3][i] * initial[i]) % MODULUS;

    printf("%llu\n", (unsigned long long)answer);
    return 0;
}
