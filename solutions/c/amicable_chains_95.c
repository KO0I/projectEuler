/* Project Euler 95: https://projecteuler.net/problem=95
 * A divisor sieve supplies the successor of each integer. Each vertex is
 * visited once. A repeated vertex from the current traversal identifies a
 * cycle; entering an older traversal cannot discover a new cycle. */
#include "euler_extra.h"

int main(void) {
    enum { LIMIT = 1000000 };
    unsigned *sum = euler_calloc(LIMIT + 1, sizeof *sum);
    unsigned *visit = euler_calloc(LIMIT + 1, sizeof *visit);
    unsigned *depth = euler_calloc(LIMIT + 1, sizeof *depth);
    for (unsigned d = 1; d <= LIMIT / 2; ++d)
        for (unsigned n = 2 * d; n <= LIMIT; n += d) sum[n] += d;
    unsigned best_length = 0, best_member = 0;
    for (unsigned start = 2; start <= LIMIT; ++start) {
        if (visit[start]) continue;
        unsigned n = start, step = 0;
        while (n && n <= LIMIT && !visit[n]) {
            visit[n] = start; depth[n] = step++;
            n = sum[n];
        }
        if (!n || n > LIMIT || visit[n] != start) continue;
        unsigned length = step - depth[n], minimum = n;
        for (unsigned m = sum[n]; m != n; m = sum[m])
            if (m < minimum) minimum = m;
        if (length > best_length ||
            (length == best_length && minimum < best_member)) {
            best_length = length; best_member = minimum;
        }
    }
    free(sum); free(visit); free(depth);
    printf("%u\n", best_member);
    return 0;
}
