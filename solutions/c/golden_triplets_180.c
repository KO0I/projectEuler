/* Project Euler 180: https://projecteuler.net/problem=180 */
#include "euler_extra.h"

typedef struct { uint64_t numerator, denominator; } fraction180;
static fraction180 sums180[400000];
static size_t count180;

static uint64_t gcd180(uint64_t a, uint64_t b) {
    while (b) { uint64_t r = a % b; a = b; b = r; }
    return a;
}

static fraction180 reduce180(uint64_t numerator, uint64_t denominator) {
    uint64_t divisor = gcd180(numerator, denominator);
    return (fraction180){numerator / divisor, denominator / divisor};
}

static bool valid180(fraction180 value) {
    return value.numerator > 0 && value.numerator < value.denominator &&
           value.denominator <= 35;
}

static uint64_t square_root180(uint64_t value) {
    uint64_t root = (uint64_t)sqrt((double)value);
    while ((root + 1) * (root + 1) <= value) ++root;
    while (root * root > value) --root;
    return root * root == value ? root : 0;
}

static void add_sum180(fraction180 x, fraction180 y, fraction180 z) {
    uint64_t denominator = x.denominator * y.denominator * z.denominator;
    uint64_t numerator = x.numerator * y.denominator * z.denominator +
                         y.numerator * x.denominator * z.denominator +
                         z.numerator * x.denominator * y.denominator;
    sums180[count180++] = reduce180(numerator, denominator);
}

static int compare180(const void *left, const void *right) {
    const fraction180 *a = left, *b = right;
    uint64_t lhs = a->numerator * b->denominator;
    uint64_t rhs = b->numerator * a->denominator;
    return lhs < rhs ? -1 : lhs > rhs;
}

int main(void) {
    fraction180 values[400];
    unsigned count = 0;
    for (unsigned denominator = 2; denominator <= 35; ++denominator)
        for (unsigned numerator = 1; numerator < denominator; ++numerator)
            if (gcd180(numerator, denominator) == 1)
                values[count++] = (fraction180){numerator, denominator};
    for (unsigned i = 0; i < count; ++i) for (unsigned j = i; j < count; ++j) {
        fraction180 x = values[i], y = values[j], z;
        z = reduce180(x.numerator * y.denominator + y.numerator * x.denominator,
                      x.denominator * y.denominator);
        if (valid180(z)) add_sum180(x, y, z);
        z = reduce180(x.numerator * y.numerator,
                      x.numerator * y.denominator + y.numerator * x.denominator);
        if (valid180(z)) add_sum180(x, y, z);
        uint64_t squared = x.numerator * x.numerator * y.denominator * y.denominator +
                           y.numerator * y.numerator * x.denominator * x.denominator;
        uint64_t root = square_root180(squared);
        if (root) {
            z = reduce180(root, x.denominator * y.denominator);
            if (valid180(z)) add_sum180(x, y, z);
            z = reduce180(x.numerator * y.numerator, root);
            if (valid180(z)) add_sum180(x, y, z);
        }
    }
    qsort(sums180, count180, sizeof(*sums180), compare180);
    mpq_t total, term;
    mpq_inits(total, term, NULL);
    for (size_t i = 0; i < count180; ++i) {
        if (i && compare180(&sums180[i - 1], &sums180[i]) == 0) continue;
        mpq_set_ui(term, sums180[i].numerator, sums180[i].denominator);
        mpq_add(total, total, term);
    }
    mpz_add(mpq_numref(total), mpq_numref(total), mpq_denref(total));
    gmp_printf("%Zd\n", mpq_numref(total));
    mpq_clears(total, term, NULL);
    return 0;
}
