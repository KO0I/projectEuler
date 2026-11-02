/* Project Euler 167: Investigating Ulam Sequences
 *
 * For U(2,v) with odd v, generate the short prefix until the sole even term
 * 2(v+1) appears.  Beyond it, membership of successive odd numbers follows a
 * finite XOR recurrence whose state is the last v+1 membership bits.
 *
 * Store the first index of each bit state to locate the eventual cycle.  A
 * prefix count of set bits gives how many Ulam terms occur before and within
 * the cycle, so the 10^11-th term is reached by whole-cycle arithmetic plus a
 * short local scan.  This proves the extrapolation instead of assuming a
 * period observed for a finite sample. */
#include "euler_extra.h"

static uint64_t kth167(unsigned v, uint64_t rank) {
    unsigned gap = v + 1;
    unsigned even = 2 * gap;
    bool member[128] = {false};
    unsigned char representations[256] = {0};
    unsigned terms[128] = {2, v};
    unsigned used = 2, last = v;
    member[2] = member[v] = true;
    ++representations[2 + v];
    while (last < even) {
        unsigned next = last + 1;
        while (representations[next] != 1) ++next;
        for (unsigned i = 0; i < used; ++i)
            if (representations[next + terms[i]] < 2) ++representations[next + terms[i]];
        terms[used++] = next;
        member[next] = true;
        last = next;
    }

    uint32_t state = 0;
    unsigned seed_ones = 0;
    for (unsigned index = 0; index < gap; ++index) {
        bool bit = member[2 * index + 1];
        state = (state << 1) | bit;
        seed_ones += bit;
    }
    uint64_t even_rank = 2 + seed_ones;
    if (rank == 1) return 2;
    if (rank == even_rank) return even;
    uint64_t odd_rank = rank < even_rank ? rank - 1 : rank - 2;

    size_t states = (size_t)1 << gap;
    uint32_t *seen = malloc(states * sizeof(*seen));
    uint32_t *prefix = malloc((states + gap + 1) * sizeof(*prefix));
    if (!seen || !prefix) exit(EXIT_FAILURE);
    for (size_t i = 0; i < states; ++i) seen[i] = UINT32_MAX;
    prefix[0] = 0;
    for (unsigned i = 0; i < gap; ++i)
        prefix[i + 1] = prefix[i] + member[2 * i + 1];
    uint32_t generated = gap;
    uint32_t mask = (1u << gap) - 1;
    seen[state] = generated;
    uint32_t cycle_start, cycle_end;
    for (;;) {
        unsigned next = ((state >> (gap - 1)) ^ state) & 1u;
        state = ((state << 1) & mask) | next;
        prefix[generated + 1] = prefix[generated] + next;
        ++generated;
        if (seen[state] != UINT32_MAX) {
            cycle_start = seen[state];
            cycle_end = generated;
            break;
        }
        seen[state] = generated;
    }
    uint64_t index;
    if (odd_rank <= prefix[cycle_start]) {
        index = 0;
        while (prefix[index + 1] < odd_rank) ++index;
    } else {
        uint64_t cycle_ones = prefix[cycle_end] - prefix[cycle_start];
        uint64_t remaining = odd_rank - prefix[cycle_start] - 1;
        uint64_t cycles = remaining / cycle_ones;
        uint64_t within = remaining % cycle_ones + 1;
        uint32_t local = cycle_start;
        while ((uint64_t)(prefix[local + 1] - prefix[cycle_start]) < within) ++local;
        index = local + cycles * (cycle_end - cycle_start);
    }
    free(seen);
    free(prefix);
    return 2 * index + 1;
}

int main(void) {
    uint64_t answer = 0;
    for (unsigned n = 2; n <= 10; ++n) answer += kth167(2 * n + 1, 100000000000ULL);
    printf("%" PRIu64 "\n", answer);
    return 0;
}
