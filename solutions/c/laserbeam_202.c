#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 202: Laserbeam.
 *
 * Unfolding each reflection turns the triangular mirror path into a straight
 * segment through a triangular lattice.  A path with N reflections reaches a
 * lattice vertex s*CA+t*CB where
 *
 *       s+t = (N+3)/2 = n.
 *
 * It is another copy of vertex C exactly when s-t is divisible by 3.  Also
 * gcd(s,t)=gcd(s,n) must be one; otherwise the segment hits an earlier lattice
 * vertex (an open corner) before its intended exit.  Thus the geometric count
 * becomes the number of 1<=s<n satisfying
 *
 *       2s == n (mod 3),  gcd(s,n)=1.
 *
 * Inclusion-exclusion over the distinct prime factors of n enforces the gcd
 * condition.  For each squarefree divisor e, counting multiples e*k in one
 * residue class modulo 3 is constant time, so the huge reflection count never
 * appears as a loop bound.
 */

static uint64_t count_k_in_residue(uint64_t maximum, int residue) {
    if (residue == 0) {
        return maximum / 3;
    }
    if (maximum < (uint64_t)residue) {
        return 0;
    }
    return (maximum - (uint64_t)residue) / 3 + 1;
}

static uint64_t multiples_in_residue(uint64_t n, uint64_t divisor, int residue) {
    const uint64_t maximum_k = (n - 1) / divisor;
    const int divisor_mod3 = (int)(divisor % 3);
    if (divisor_mod3 == 0) {
        return residue == 0 ? maximum_k : 0;
    }
    /* 1 is self-inverse mod 3; 2 is also self-inverse because 2*2==1. */
    const int k_residue = divisor_mod3 == 1 ? residue : (2 * residue) % 3;
    return count_k_in_residue(maximum_k, k_residue);
}

static uint64_t path_count(uint64_t reflections) {
    const uint64_t n = (reflections + 3) / 2;
    uint64_t remaining = n;
    uint64_t factors[16];
    int factor_count = 0;
    for (uint64_t p = 2; p * p <= remaining; p += p == 2 ? 1 : 2) {
        if (remaining % p == 0) {
            factors[factor_count++] = p;
            do {
                remaining /= p;
            } while (remaining % p == 0);
        }
    }
    if (remaining > 1) {
        factors[factor_count++] = remaining;
    }

    /* Solve 2s == n mod 3 by multiplying by the inverse 2. */
    const int target_residue = (int)((2 * (n % 3)) % 3);
    int64_t total = 0;
    const unsigned subsets = 1U << factor_count;
    for (unsigned mask = 0; mask < subsets; ++mask) {
        uint64_t divisor = 1;
        int parity = 0;
        for (int i = 0; i < factor_count; ++i) {
            if ((mask & (1U << i)) != 0U) {
                divisor *= factors[i];
                parity ^= 1;
            }
        }
        const uint64_t count = multiples_in_residue(n, divisor, target_residue);
        total += parity ? -(int64_t)count : (int64_t)count;
    }
    return (uint64_t)total;
}

int main(void) {
    printf("%llu\n", (unsigned long long)path_count(UINT64_C(12017639147)));
    return 0;
}
