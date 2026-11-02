/* Project Euler 149: Maximum-sum Subsequence
 *
 * First generate all four million lagged-Fibonacci values in row-major order.
 * Every horizontal, vertical, and both diagonal line is then scanned with
 * Kadane's recurrence: extend a nonnegative prefix, otherwise restart at the
 * current cell.  Starting `best` at INT64_MIN correctly handles an all-negative
 * line rather than allowing an empty subsequence.
 *
 * Launching scans from each boundary covers every line once (corner lines may
 * be harmlessly revisited).  Sums use int64_t even though each grid entry is
 * a signed 20-bit value. */
#include "euler_extra.h"

enum { SIDE149 = 2000 };
static int *grid149;

static int64_t line_sum(int row, int col, int dr, int dc) {
    int64_t best = INT64_MIN, current = 0;
    while (row >= 0 && row < SIDE149 && col >= 0 && col < SIDE149) {
        current += grid149[row * SIDE149 + col];
        if (current > best) best = current;
        if (current < 0) current = 0;
        row += dr; col += dc;
    }
    return best;
}

int main(void) {
    const unsigned size = SIDE149 * SIDE149;
    grid149 = malloc(size * sizeof(*grid149));
    if (!grid149) return EXIT_FAILURE;
    for (unsigned k = 1; k <= size; ++k) {
        int64_t value;
        if (k <= 55) value = (100003 - 200003LL * k + 300007LL * k * k * k) % 1000000;
        else value = (grid149[k - 25] + grid149[k - 56] + 1000000) % 1000000;
        if (value < 0) value += 1000000;
        grid149[k - 1] = (int)value - 500000;
    }
    int64_t best = INT64_MIN;
    for (int i = 0; i < SIDE149; ++i) {
        int64_t values[] = {line_sum(i, 0, 0, 1), line_sum(0, i, 1, 0),
                            line_sum(i, 0, 1, 1), line_sum(0, i, 1, 1),
                            line_sum(i, 0, -1, 1), line_sum(SIDE149 - 1, i, -1, 1)};
        for (unsigned j = 0; j < sizeof(values)/sizeof(values[0]); ++j)
            if (values[j] > best) best = values[j];
    }
    printf("%" PRId64 "\n", best);
    free(grid149);
    return 0;
}
