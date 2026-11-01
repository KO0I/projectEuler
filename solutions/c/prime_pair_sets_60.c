/* Project Euler 60: https://projecteuler.net/problem=60
 * Build a graph whose edges mean that both concatenations are prime.
 * Increasing candidate lists enumerate each five-clique exactly once.
 * Discover an upper bound by doubling a prime limit; then search all
 * primes below the discovered sum. Every better set must lie in that
 * final graph, so the last search establishes the global minimum. */
#include "euler_extra.h"

typedef struct {
    size_t count;
    unsigned *primes;
    unsigned char *adjacent;
    size_t *scratch;
} Graph;

static uint64_t concatenate(unsigned a, unsigned b) {
    uint64_t power = 10;
    while (power <= b) power *= 10;
    return power * a + b;
}

static Graph make_graph(unsigned limit, uint64_t best) {
    unsigned char *sieve = euler_sieve(limit);
    size_t count = 0;
    for (unsigned p = 3; p <= limit; p += 2) if (p != 5 && sieve[p]) ++count;
    Graph graph = {
        count,
        euler_malloc(count * sizeof(unsigned)),
        euler_calloc(count, count),
        euler_calloc(5 * count, sizeof(size_t))
    };
    size_t index = 0;
    for (unsigned p = 3; p <= limit; p += 2)
        if (p != 5 && sieve[p]) graph.primes[index++] = p;
    free(sieve);
    /* 2 and 5 cannot occur: another prime concatenated with either ends
     * in an even digit or a five. Opposite nonzero residues mod 3 also
     * cannot form an edge, because decimal concatenation preserves them. */
    for (size_t i = 0; i < count; ++i)
        for (size_t j = i + 1; j < count; ++j) {
            unsigned a = graph.primes[i], b = graph.primes[j];
            if ((uint64_t)a + b >= best || (a + b) % 3 == 0) continue;
            if (euler_is_prime(concatenate(a, b)) && euler_is_prime(concatenate(b, a))) {
                graph.adjacent[i * count + j] = 1;
                graph.adjacent[j * count + i] = 1;
            }
        }
    return graph;
}

static void release_graph(Graph *graph) {
    free(graph->scratch);
    free(graph->adjacent);
    free(graph->primes);
}

static void search(const Graph *graph, unsigned depth, uint64_t sum,
                   const size_t *candidates, size_t count, uint64_t *best) {
    unsigned needed = 5 - depth;
    if (count < needed) return;
    uint64_t lower = sum;
    for (unsigned j = 0; j < needed; ++j) lower += graph->primes[candidates[j]];
    if (lower >= *best) return;
    if (needed == 1) { *best = lower; return; }
    size_t *next = graph->scratch + depth * graph->count;
    for (size_t i = 0; i + needed <= count; ++i) {
        lower = sum;
        for (unsigned j = 0; j < needed; ++j) lower += graph->primes[candidates[i + j]];
        if (lower >= *best) break;
        size_t vertex = candidates[i], next_count = 0;
        for (size_t j = i + 1; j < count; ++j)
            if (graph->adjacent[vertex * graph->count + candidates[j]])
                next[next_count++] = candidates[j];
        search(graph, depth + 1, sum + graph->primes[vertex], next, next_count, best);
    }
}

static void search_graph(Graph *graph, uint64_t *best) {
    size_t *candidates = euler_malloc(graph->count * sizeof *candidates);
    for (size_t i = 0; i < graph->count; ++i) candidates[i] = i;
    search(graph, 0, 0, candidates, graph->count, best);
    free(candidates);
}

int main(void) {
    uint64_t best = UINT64_MAX;
    unsigned limit = 1000;
    for (;;) {
        Graph graph = make_graph(limit, best);
        search_graph(&graph, &best);
        release_graph(&graph);
        if (best != UINT64_MAX) break;
        if (limit > UINT_MAX / 2) euler_fail("Prime-pair discovery bound overflow");
        limit *= 2;
    }
    if (best > UINT_MAX) euler_fail("Prime-pair proof bound exceeds the sieve range");
    Graph complete = make_graph((unsigned)best - 1, best);
    search_graph(&complete, &best);
    release_graph(&complete);
    printf("%" PRIu64 "\n", best);
    return 0;
}
