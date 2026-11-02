/* Independent small cases for the early additions. Compile from repo root:
 * gcc -O2 -std=c11 -Wall -Wextra -Wpedantic -Werror -Isupport/c
 *     tests/c_early_examples.c -lgmp -lm -o build/agent_early/examples
 * Includes the real implementation functions rather than reimplementations. */
#include <assert.h>
#define main euler26_program
#include "../solutions/c/reciprocal_cycles_26.c"
#undef main
#define main euler54_program
#include "../solutions/c/poker_hands_54.c"
#undef main
#define main euler60_program
#include "../solutions/c/prime_pair_sets_60.c"
#undef main

static bool wins(const char *deal) {
    char line[128];
    assert(strlen(deal) < sizeof line);
    strcpy(line, deal);
    return first_player_wins(line);
}

static void check_clique_search(void) {
    /* Exhaustive five-subset oracle on small synthetic graphs exercises
     * pruning independently of the production recursive search. */
    enum { N = 9 };
    unsigned weights[N] = {3,7,11,13,17,19,23,29,31};
    unsigned char edges[N * N];
    size_t scratch[5 * N];
    Graph graph = {N, weights, edges, scratch};
    uint32_t state = 123456789;
    for (unsigned trial = 0; trial < 128; ++trial) {
        memset(edges, 0, sizeof edges);
        for (size_t i = 0; i < N; ++i)
            for (size_t j = i + 1; j < N; ++j) {
                state ^= state << 13; state ^= state >> 17; state ^= state << 5;
                bool edge = (state & 3) != 0;
                edges[i * N + j] = edges[j * N + i] = edge;
            }
        uint64_t expected = UINT64_MAX;
        for (size_t a = 0; a < N; ++a)
            for (size_t b = a + 1; b < N; ++b)
                for (size_t c = b + 1; c < N; ++c)
                    for (size_t d = c + 1; d < N; ++d)
                        for (size_t e = d + 1; e < N; ++e) {
                            size_t chosen[5] = {a,b,c,d,e};
                            bool clique = true;
                            uint64_t sum = 0;
                            for (size_t i = 0; i < 5; ++i) {
                                sum += weights[chosen[i]];
                                for (size_t j = i + 1; j < 5; ++j)
                                    if (!edges[chosen[i] * N + chosen[j]]) clique = false;
                            }
                            if (clique && sum < expected) expected = sum;
                        }
        uint64_t actual = UINT64_MAX;
        search_graph(&graph, &actual);
        assert(actual == expected);
        if (expected != UINT64_MAX) {
            actual = expected + 1;
            search_graph(&graph, &actual);
            assert(actual == expected);
        }
    }
}

int main(void) {
    assert(cycle_length(2) == 0);
    assert(cycle_length(3) == 1);
    assert(cycle_length(6) == 1);
    assert(cycle_length(7) == 6);
    assert(cycle_length(8) == 0);
    assert(cycle_length(13) == 6);
    /* The five examples in the official poker statement. */
    assert(!wins("5H 5C 6S 7S KD 2C 3S 8S 8D TD"));
    assert(wins("5D 8C 9S JS AC 2C 5C 7D 8S QH"));
    assert(!wins("2D 9C AS AH AC 3D 6D 7D TD QD"));
    assert(wins("4D 6S 9H QH QC 3D 6D 7H QD QS"));
    assert(wins("2H 2D 4C 4D 4S 3C 3D 3S 9S 9D"));
    /* Five-high straight, flush tie breakers, and both pairs before kicker. */
    assert(!wins("AH 2C 3D 4S 5H 2H 3C 4D 5S 6H"));
    assert(wins("AH KH QH JH 9H AC KC QC JC 8C"));
    assert(wins("AH AC 5H 5C 2D AS AD 4H 4C KD"));
    assert(wins("TH JH QH KH AH 9C TC JC QC KC"));
    assert(concatenate(7, 109) == 7109);
    assert(concatenate(109, 7) == 1097);
    /* The supplied four-prime clique is a direct graph-construction case. */
    Graph graph = make_graph(700, UINT64_MAX);
    static const unsigned example[] = {3,7,109,673};
    size_t indices[4] = {SIZE_MAX,SIZE_MAX,SIZE_MAX,SIZE_MAX};
    for (size_t i = 0; i < graph.count; ++i)
        for (size_t j = 0; j < 4; ++j)
            if (graph.primes[i] == example[j]) indices[j] = i;
    for (size_t i = 0; i < 4; ++i) {
        assert(indices[i] != SIZE_MAX);
        for (size_t j = i + 1; j < 4; ++j)
            assert(graph.adjacent[indices[i] * graph.count + indices[j]]);
    }
    uint64_t best = UINT64_MAX;
    search_graph(&graph, &best);
    assert(best == UINT64_MAX); /* No five-clique among primes <=700. */
    release_graph(&graph);
    check_clique_search();
    puts("early C examples passed");
    return 0;
}
