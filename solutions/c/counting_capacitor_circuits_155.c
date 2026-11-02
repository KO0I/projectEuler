/* Project Euler 155: https://projecteuler.net/problem=155 */
#include "euler_extra.h"

typedef struct { uint32_t numerator, denominator; } fraction155;
typedef struct { fraction155 *data; size_t size, capacity; } vector155;

static uint32_t gcd155(uint32_t a, uint32_t b) {
    while (b) { uint32_t r = a % b; a = b; b = r; }
    return a;
}

static fraction155 reduce155(uint64_t numerator, uint64_t denominator) {
    uint32_t n = numerator, d = denominator;
    uint32_t divisor = gcd155(n, d);
    return (fraction155){n / divisor, d / divisor};
}

static void push155(vector155 *vector, fraction155 value) {
    if (vector->size == vector->capacity) {
        size_t capacity = vector->capacity ? vector->capacity * 2 : 1024;
        fraction155 *grown = realloc(vector->data, capacity * sizeof(*grown));
        if (!grown) exit(EXIT_FAILURE);
        vector->data = grown;
        vector->capacity = capacity;
    }
    vector->data[vector->size++] = value;
}

static int compare155(const void *left, const void *right) {
    const fraction155 *a = left, *b = right;
    uint64_t lhs = (uint64_t)a->numerator * b->denominator;
    uint64_t rhs = (uint64_t)b->numerator * a->denominator;
    return lhs < rhs ? -1 : lhs > rhs;
}

static void unique155(vector155 *vector) {
    qsort(vector->data, vector->size, sizeof(*vector->data), compare155);
    size_t kept = 0;
    for (size_t i = 0; i < vector->size; ++i)
        if (!kept || compare155(&vector->data[kept - 1], &vector->data[i]) != 0)
            vector->data[kept++] = vector->data[i];
    vector->size = kept;
}

int main(void) {
    vector155 circuits[19] = {{0}};
    push155(&circuits[1], (fraction155){1, 1});
    for (unsigned size = 2; size <= 18; ++size) {
        for (unsigned left_size = 1; left_size <= size / 2; ++left_size) {
            vector155 *left = &circuits[left_size];
            vector155 *right = &circuits[size - left_size];
            for (size_t i = 0; i < left->size; ++i) for (size_t j = 0; j < right->size; ++j) {
                fraction155 a = left->data[i], b = right->data[j];
                uint64_t sum_n = (uint64_t)a.numerator * b.denominator +
                                 (uint64_t)b.numerator * a.denominator;
                uint64_t sum_d = (uint64_t)a.denominator * b.denominator;
                push155(&circuits[size], reduce155(sum_n, sum_d));
                push155(&circuits[size], reduce155((uint64_t)a.numerator * b.numerator, sum_n));
            }
        }
        unique155(&circuits[size]);
    }
    vector155 all = {0};
    for (unsigned size = 1; size <= 18; ++size)
        for (size_t i = 0; i < circuits[size].size; ++i) push155(&all, circuits[size].data[i]);
    unique155(&all);
    printf("%zu\n", all.size);
    free(all.data);
    for (unsigned size = 1; size <= 18; ++size) free(circuits[size].data);
    return 0;
}
