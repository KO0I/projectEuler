/* Project Euler 150: Sub-triangle Sums
 *
 * Generate the 500500 pseudo-random entries exactly as specified and store a
 * prefix sum for every row.  For a fixed apex (top,col), increasing `bottom`
 * adds one contiguous row slice whose endpoints expand by one each time.
 * Row prefixes make that slice O(1), so all downward triangles are examined
 * in O(ROWS^3) arithmetic instead of summing each triangle from scratch.
 *
 * Individual values are signed, and both prefix and triangle sums can exceed
 * 32 bits, hence int64_t throughout.  The allocated rows include a leading
 * zero so slice [left,right] is prefix[right+1]-prefix[left]. */
#include "euler_extra.h"

int main(void) {
    enum { ROWS = 1000 };
    int64_t **prefix = malloc(ROWS * sizeof(*prefix));
    if (!prefix) return EXIT_FAILURE;
    int64_t t = 0;
    for (unsigned row = 0; row < ROWS; ++row) {
        prefix[row] = malloc((row + 2) * sizeof(**prefix));
        if (!prefix[row]) return EXIT_FAILURE;
        prefix[row][0] = 0;
        for (unsigned col = 0; col <= row; ++col) {
            t = (615949 * t + 797807) % (1 << 20);
            prefix[row][col + 1] = prefix[row][col] + t - (1 << 19);
        }
    }
    int64_t best = INT64_MAX;
    for (unsigned top = 0; top < ROWS; ++top) {
        for (unsigned col = 0; col <= top; ++col) {
            int64_t sum = 0;
            for (unsigned bottom = top; bottom < ROWS; ++bottom) {
                sum += prefix[bottom][col + bottom - top + 1] - prefix[bottom][col];
                if (sum < best) best = sum;
            }
        }
    }
    printf("%" PRId64 "\n", best);
    for (unsigned row = 0; row < ROWS; ++row) free(prefix[row]);
    free(prefix);
    return 0;
}
