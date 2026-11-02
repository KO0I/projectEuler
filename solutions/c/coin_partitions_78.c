/* https://projecteuler.net/problem=78
 * Euler's pentagonal recurrence is linear, so reducing each p(n) modulo one
 * million preserves the divisibility test. The signs repeat +,+,-,- at the
 * generalized pentagonal offsets k*(3*k-1)/2 and k*(3*k+1)/2. */
#include "euler_extra.h"

static unsigned p78_first_divisible(unsigned modulus) {
    if (modulus == 0) euler_fail("Partition modulus must be positive");
    size_t capacity = 1024;
    unsigned *partitions = euler_malloc(capacity * sizeof *partitions);
    partitions[0] = 1 % modulus;
    for (unsigned n = 1;; ++n) {
        if (n == capacity) {
            capacity *= 2;
            unsigned *larger = realloc(partitions, capacity * sizeof *partitions);
            if (!larger) euler_fail("Partition allocation failed");
            partitions = larger;
        }
        int64_t total = 0;
        for (uint64_t k = 1;; ++k) {
            uint64_t first = k * (3 * k - 1) / 2, second = first + k;
            if (first > n) break;
            int sign = k % 2 ? 1 : -1;
            total += sign * (int64_t)partitions[n - first];
            if (second <= n) total += sign * (int64_t)partitions[n - second];
        }
        total %= modulus;
        if (total < 0) total += modulus;
        partitions[n] = (unsigned)total;
        if (total == 0) { free(partitions); return n; }
        if (n == UINT_MAX) euler_fail("Partition index range exceeded");
    }
}

int main(void) {
    printf("%u\n", p78_first_divisible(1000000));
    return 0;
}
