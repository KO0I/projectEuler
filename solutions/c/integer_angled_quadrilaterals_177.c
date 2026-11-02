/* Project Euler 177: https://projecteuler.net/problem=177 */
#include "euler_extra.h"

typedef struct { double ratio; uint8_t a, c; } ratio177;
typedef struct { uint8_t angle[8]; } shape177;
enum { SHAPES177 = 1 << 20 };
static shape177 *shapes177;

static int compare_ratio177(const void *left, const void *right) {
    double a = ((const ratio177 *)left)->ratio;
    double b = ((const ratio177 *)right)->ratio;
    return a < b ? -1 : a > b;
}

static bool less177(const uint8_t *a, const uint8_t *b) {
    for (unsigned i = 0; i < 8; ++i) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return false;
}

static void canonical177(uint8_t output[8], const uint8_t input[8]) {
    memcpy(output, input, 8);
    uint8_t candidate[8], reverse[8];
    for (unsigned i = 0; i < 8; ++i) reverse[i] = input[7 - i];
    for (unsigned reflection = 0; reflection < 2; ++reflection) {
        const uint8_t *source = reflection ? reverse : input;
        for (unsigned rotation = 0; rotation < 8; rotation += 2) {
            for (unsigned i = 0; i < 8; ++i) candidate[i] = source[(i + rotation) & 7];
            if (less177(candidate, output)) memcpy(output, candidate, 8);
        }
    }
}

static bool insert177(const uint8_t angles[8]) {
    uint64_t packed = 0;
    for (unsigned i = 0; i < 8; ++i) packed = (packed << 8) | angles[i];
    uint64_t hash = packed;
    hash ^= hash >> 30; hash *= 0xbf58476d1ce4e5b9ULL;
    hash ^= hash >> 27; hash *= 0x94d049bb133111ebULL;
    hash ^= hash >> 31;
    size_t slot = hash & (SHAPES177 - 1);
    while (shapes177[slot].angle[0]) {
        if (memcmp(shapes177[slot].angle, angles, 8) == 0) return false;
        slot = (slot + 1) & (SHAPES177 - 1);
    }
    memcpy(shapes177[slot].angle, angles, 8);
    return true;
}

int main(void) {
    double log_sine[180];
    for (unsigned angle = 1; angle < 180; ++angle)
        log_sine[angle] = log(sin(angle * acos(-1.0) / 180.0));
    shapes177 = calloc(SHAPES177, sizeof(*shapes177));
    if (!shapes177) return EXIT_FAILURE;
    unsigned answer = 0;
    for (unsigned u = 2; u <= 178; ++u) {
        unsigned v = 180 - u;
        size_t count = (size_t)(u - 1) * (v - 1), used = 0;
        ratio177 *table = malloc(count * sizeof(*table));
        if (!table) return EXIT_FAILURE;
        for (unsigned a = 1; a < v; ++a) for (unsigned c = 1; c < u; ++c) {
            table[used++] = (ratio177){log_sine[u - c] + log_sine[a] -
                                      log_sine[c] - log_sine[v - a], a, c};
        }
        qsort(table, count, sizeof(*table), compare_ratio177);
        for (unsigned p = 1; p < u; ++p) {
            unsigned q = u - p;
            for (unsigned r = 1; r < v; ++r) {
                unsigned s = v - r;
                double target = log_sine[p] + log_sine[r] - log_sine[q] - log_sine[s];
                size_t low = 0, high = count;
                while (low < high) {
                    size_t mid = low + (high - low) / 2;
                    if (table[mid].ratio < target - 1e-11) low = mid + 1;
                    else high = mid;
                }
                for (size_t i = low; i < count && table[i].ratio <= target + 1e-11; ++i) {
                    if (fabs(table[i].ratio - target) > 1e-12) continue;
                    unsigned a = table[i].a, c = table[i].c;
                    uint8_t raw[8] = {a, p, q, r, s, c, u - c, v - a}, canonical[8];
                    canonical177(canonical, raw);
                    answer += insert177(canonical);
                }
            }
        }
        free(table);
    }
    printf("%u\n", answer);
    free(shapes177);
    return 0;
}
