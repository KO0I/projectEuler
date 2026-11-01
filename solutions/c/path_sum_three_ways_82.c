/* https://projecteuler.net/problem=82
 * Process columns from left to right. Enter each row from its left neighbour,
 * then sweep down and up to allow a vertical segment within the new column.
 * Positive weights make any vertical reversal or repeated visit unnecessary. */
#include "euler_extra.h"

static void p82_read_matrix(unsigned *matrix, size_t side, const char *path) {
    FILE *file = euler_open(path);
    for (size_t row = 0; row < side; ++row) {
        for (size_t col = 0; col < side; ++col) {
            unsigned *cell = &matrix[row * side + col];
            if (fscanf(file, "%u", cell) != 1 || *cell == 0 || *cell > 1000000)
                euler_fail("Expected positive matrix entries");
            if (col + 1 < side && fgetc(file) != ',') euler_fail("Expected matrix comma");
        }
        int separator = fgetc(file);
        if (separator == '\r') separator = fgetc(file);
        if (separator != '\n' && !(row + 1 == side && separator == EOF))
            euler_fail("Unexpected matrix row length");
    }
    char extra;
    if (fscanf(file, " %c", &extra) != EOF || ferror(file)) euler_fail("Unexpected matrix data");
    fclose(file);
}

static uint64_t p82_minimum(const unsigned *matrix, size_t side) {
    uint64_t *cost = euler_malloc(side * sizeof *cost);
    for (size_t row = 0; row < side; ++row) cost[row] = matrix[row * side];
    for (size_t col = 1; col < side; ++col) {
        for (size_t row = 0; row < side; ++row) cost[row] += matrix[row * side + col];
        for (size_t row = 1; row < side; ++row) {
            uint64_t down = cost[row - 1] + matrix[row * side + col];
            if (down < cost[row]) cost[row] = down;
        }
        for (size_t row = side - 1; row > 0; --row) {
            uint64_t up = cost[row] + matrix[(row - 1) * side + col];
            if (up < cost[row - 1]) cost[row - 1] = up;
        }
    }
    uint64_t answer = cost[0];
    for (size_t row = 1; row < side; ++row) if (cost[row] < answer) answer = cost[row];
    free(cost);
    return answer;
}

int main(void) {
    unsigned matrix[80 * 80];
    p82_read_matrix(matrix, 80, "tests/data/p082_matrix.txt");
    printf("%" PRIu64 "\n", p82_minimum(matrix, 80));
    return 0;
}
