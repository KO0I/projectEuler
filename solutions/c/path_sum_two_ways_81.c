/* https://projecteuler.net/problem=81
 * Right/down paths form a directed acyclic graph. A one-row dynamic
 * program retains the best cost from above and the newly updated cost at left. */
#include "euler_extra.h"

static void p81_read_matrix(unsigned *matrix, size_t side, const char *path) {
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

static uint64_t p81_minimum(const unsigned *matrix, size_t side) {
    uint64_t *cost = euler_calloc(side, sizeof *cost);
    for (size_t row = 0; row < side; ++row) {
        for (size_t col = 0; col < side; ++col) {
            if (row == 0 && col > 0) cost[col] = cost[col - 1];
            else if (row > 0 && col > 0 && cost[col - 1] < cost[col]) cost[col] = cost[col - 1];
            cost[col] += matrix[row * side + col];
        }
    }
    uint64_t answer = cost[side - 1];
    free(cost);
    return answer;
}

int main(void) {
    unsigned matrix[80 * 80];
    p81_read_matrix(matrix, 80, "tests/data/p081_matrix.txt");
    printf("%" PRIu64 "\n", p81_minimum(matrix, 80));
    return 0;
}
