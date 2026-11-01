/* https://projecteuler.net/problem=83
 * Dijkstra settles the cheapest unvisited vertex before relaxing its four
 * neighbours. The 80x80 grid is small enough for the simple O(V^2) minimum
 * scan; uint64_t costs retain exact sums of positive input weights. */
#include "euler_extra.h"

static void p83_read_matrix(unsigned *matrix, size_t side, const char *path) {
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

static uint64_t p83_minimum(const unsigned *matrix, size_t side) {
    const size_t count = side * side;
    uint64_t *cost = euler_malloc(count * sizeof *cost);
    bool *settled = euler_calloc(count, sizeof *settled);
    for (size_t i = 0; i < count; ++i) cost[i] = UINT64_MAX;
    cost[0] = matrix[0];
    for (size_t step = 0; step < count; ++step) {
        size_t current = count;
        for (size_t i = 0; i < count; ++i)
            if (!settled[i] && (current == count || cost[i] < cost[current])) current = i;
        if (current == count || cost[current] == UINT64_MAX) euler_fail("Matrix destination is unreachable");
        if (current == count - 1) break;
        settled[current] = true;
        size_t neighbour[4], used = 0;
        if (current % side > 0) neighbour[used++] = current - 1;
        if (current % side + 1 < side) neighbour[used++] = current + 1;
        if (current >= side) neighbour[used++] = current - side;
        if (current + side < count) neighbour[used++] = current + side;
        for (size_t i = 0; i < used; ++i) {
            size_t next = neighbour[i];
            uint64_t candidate = cost[current] + matrix[next];
            if (!settled[next] && candidate < cost[next]) cost[next] = candidate;
        }
    }
    uint64_t answer = cost[count - 1];
    free(settled); free(cost);
    return answer;
}

int main(void) {
    unsigned matrix[80 * 80];
    p83_read_matrix(matrix, 80, "tests/data/p083_matrix.txt");
    printf("%" PRIu64 "\n", p83_minimum(matrix, 80));
    return 0;
}
