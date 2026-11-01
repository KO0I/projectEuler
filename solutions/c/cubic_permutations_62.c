/* https://projecteuler.net/problem=62
 * Sort complete digit-length buckets of cubes by digit multiplicities. Waiting
 * until a bucket closes is essential: five matches might later become six.
 * Five bits per digit count cover every decimal uint64_t value. */
#include "euler_extra.h"

typedef struct { uint64_t signature, cube; } P62Cube;

static uint64_t p62_signature(uint64_t n) {
    uint64_t key = 0;
    do { key += UINT64_C(1) << (5 * (n % 10)); n /= 10; } while (n);
    return key;
}

static int p62_compare(const void *left, const void *right) {
    const P62Cube *a = left, *b = right;
    return (a->signature > b->signature) - (a->signature < b->signature);
}

static uint64_t p62_bucket(P62Cube *cubes, size_t count, size_t target) {
    qsort(cubes, count, sizeof *cubes, p62_compare);
    uint64_t best = UINT64_MAX;
    for (size_t first = 0; first < count;) {
        size_t end = first + 1;
        uint64_t smallest = cubes[first].cube;
        while (end < count && cubes[end].signature == cubes[first].signature) {
            if (cubes[end].cube < smallest) smallest = cubes[end].cube;
            ++end;
        }
        if (end - first == target && smallest < best) best = smallest;
        first = end;
    }
    return best;
}

int main(void) {
    size_t count = 0, capacity = 128;
    P62Cube *cubes = euler_malloc(capacity * sizeof *cubes);
    uint64_t next_length = 10;
    for (uint64_t n = 1;; ++n) {
        if (n > UINT64_MAX / n / n) euler_fail("Cube exceeds uint64_t search range");
        uint64_t cube = n * n * n;
        if (cube >= next_length) {
            uint64_t answer = p62_bucket(cubes, count, 5);
            if (answer != UINT64_MAX) {
                printf("%" PRIu64 "\n", answer); free(cubes); return 0;
            }
            count = 0;
            if (next_length > UINT64_MAX / 10) euler_fail("Decimal bucket limit exceeded");
            next_length *= 10;
        }
        if (count == capacity) {
            capacity *= 2;
            P62Cube *larger = realloc(cubes, capacity * sizeof *cubes);
            if (!larger) euler_fail("Cube allocation failed");
            cubes = larger;
        }
        cubes[count++] = (P62Cube){p62_signature(cube), cube};
    }
}
