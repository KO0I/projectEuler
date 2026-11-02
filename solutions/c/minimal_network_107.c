/* Project Euler 107: https://projecteuler.net/problem=107
 * A connected minimum-weight subgraph with positive edges is a spanning
 * tree. Prim's algorithm finds its weight; subtract it from all edges. */
#include "euler_extra.h"

int main(void) {
    enum { VERTICES = 40 };
    uint64_t weight[VERTICES][VERTICES], distance[VERTICES];
    bool chosen[VERTICES] = {false};
    FILE *file = euler_open("tests/data/p107_network.txt");
    char line[4096];
    for (unsigned r = 0; r < VERTICES; ++r) {
        if (!fgets(line, sizeof line, file)) euler_fail("Truncated network input");
        char *p = line;
        for (unsigned c = 0; c < VERTICES; ++c) {
            while (isspace((unsigned char)*p)) ++p;
            if (*p == '-') { weight[r][c] = 0; ++p; }
            else {
                char *end;
                if (!isdigit((unsigned char)*p)) euler_fail("Invalid network weight");
                errno = 0;
                unsigned long value = strtoul(p, &end, 10);
                if (p == end || errno || !value || value > UINT32_MAX)
                    euler_fail("Invalid network weight");
                weight[r][c] = value; p = end;
            }
            while (isspace((unsigned char)*p)) ++p;
            if (c + 1 < VERTICES) {
                if (*p != ',') euler_fail("Invalid network separator");
                ++p;
            } else if (*p) euler_fail("Extra network column");
        }
    }
    int ch;
    while ((ch = fgetc(file)) != EOF) if (!isspace((unsigned char)ch))
        euler_fail("Extra network row");
    if (ferror(file)) euler_fail("Network input read failed");
    fclose(file);
    uint64_t total = 0, tree = 0;
    for (unsigned r = 0; r < VERTICES; ++r) {
        if (weight[r][r]) euler_fail("Network diagonal must be empty");
        for (unsigned c = r + 1; c < VERTICES; ++c) {
            if (weight[r][c] != weight[c][r]) euler_fail("Network must be symmetric");
            total += weight[r][c];
        }
        distance[r] = UINT64_MAX;
    }
    distance[0] = 0;
    for (unsigned step = 0; step < VERTICES; ++step) {
        unsigned vertex = VERTICES;
        for (unsigned v = 0; v < VERTICES; ++v)
            if (!chosen[v] && (vertex == VERTICES || distance[v] < distance[vertex])) vertex = v;
        if (vertex == VERTICES || distance[vertex] == UINT64_MAX)
            euler_fail("Network is disconnected");
        chosen[vertex] = true; tree += distance[vertex];
        for (unsigned v = 0; v < VERTICES; ++v)
            if (!chosen[v] && weight[vertex][v] && weight[vertex][v] < distance[v])
                distance[v] = weight[vertex][v];
    }
    printf("%" PRIu64 "\n", total - tree);
    return 0;
}
