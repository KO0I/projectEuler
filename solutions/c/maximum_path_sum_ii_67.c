/* https://projecteuler.net/problem=67
 * Bottom-up dynamic programming replaces each entry by its value plus the
 * larger of its two reachable children. The 100-row input has 5050 entries. */
#include "euler_extra.h"

static unsigned p67_maximum(unsigned *triangle, size_t rows) {
    for (size_t row = rows - 1; row > 0; --row) {
        size_t parent = row * (row - 1) / 2, child = row * (row + 1) / 2;
        for (size_t col = 0; col < row; ++col) {
            unsigned left = triangle[child + col], right = triangle[child + col + 1];
            triangle[parent + col] += left > right ? left : right;
        }
    }
    return triangle[0];
}

int main(void) {
    unsigned triangle[5050];
    FILE *file = euler_open("tests/data/p067_triangle.txt");
    for (size_t i = 0; i < 5050; ++i)
        if (fscanf(file, "%u", &triangle[i]) != 1 || triangle[i] > 99)
            euler_fail("Expected 5050 two-digit triangle entries");
    char extra;
    if (fscanf(file, " %c", &extra) != EOF || ferror(file)) euler_fail("Unexpected triangle data");
    fclose(file);
    printf("%u\n", p67_maximum(triangle, 100));
    return 0;
}
