/* Project Euler 209: https://projecteuler.net/problem=209

   The condition couples truth-table entries x and T(x), where
     T(a,b,c,d,e,f)=(b,c,d,e,f,a XOR (b AND c)).
   T is a permutation of the 64 inputs, hence decomposes into disjoint cycles.
   On each cycle the truth values form a circular binary string with no adjacent
   ones.  A length-L cycle has F_(L-1)+F_(L+1) such strings; independent cycles
   multiply. */
#include "euler_extra.h"

static unsigned transition209(unsigned value) {
    unsigned a = (value >> 5) & 1, b = (value >> 4) & 1, c = (value >> 3) & 1;
    return ((value << 1) & 63) | (a ^ (b & c));
}

int main(void) {
    uint64_t fibonacci[66] = {0, 1};
    for (unsigned i = 2; i < 66; ++i) fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
    bool visited[64] = {false};
    uint64_t answer = 1;
    for (unsigned start = 0; start < 64; ++start) {
        if (visited[start]) continue;
        unsigned length = 0, current = start;
        do { visited[current] = true; ++length; current = transition209(current); } while (current != start);
        answer *= fibonacci[length - 1] + fibonacci[length + 1];
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
