/* https://projecteuler.net/problem=74
 * Cache the count of distinct terms before the first repetition. A traversal
 * either reaches a cached tail or discovers a cycle in its own path; cycle
 * vertices receive the cycle length, then prefix lengths propagate backwards.
 * Every successor is <=7*9!: starts have six digits and that bound has seven. */
#include "euler_extra.h"

static unsigned p74_next(unsigned n) {
    static const unsigned factorial[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
    unsigned sum = 0;
    do { sum += factorial[n % 10]; n /= 10; } while (n);
    return sum;
}

static unsigned p74_count(unsigned start_limit, unsigned wanted) {
    const size_t capacity = 7 * 362880 + 1;
    if (start_limit > capacity) euler_fail("Starting range exceeds the proven chain bound");
    unsigned *length = euler_calloc(capacity, sizeof *length);
    unsigned *seen = euler_calloc(capacity, sizeof *seen);
    unsigned *position = euler_malloc(capacity * sizeof *position);
    unsigned *path = euler_malloc(capacity * sizeof *path);
    unsigned answer = 0;
    for (unsigned start = 1; start < start_limit; ++start) {
        if (!length[start]) {
            unsigned n = start, count = 0;
            while (!length[n] && seen[n] != start) {
                seen[n] = start; position[n] = count; path[count++] = n;
                n = p74_next(n);
            }
            unsigned tail;
            if (length[n]) {
                tail = length[n];
            } else {
                unsigned first = position[n], cycle = count - first;
                for (unsigned i = first; i < count; ++i) length[path[i]] = cycle;
                count = first; tail = cycle;
            }
            while (count) length[path[--count]] = ++tail;
        }
        answer += length[start] == wanted;
    }
    free(path); free(position); free(seen); free(length);
    return answer;
}

int main(void) {
    printf("%u\n", p74_count(1000000, 60));
    return 0;
}
