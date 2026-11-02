/* Project Euler 93: https://projecteuler.net/problem=93
 * Recursively combine each pair of values using all binary operations.
 * Every binary expression tree is represented; reduced rational numbers
 * preserve exact equality, including intermediate fractional results. */
#include "euler_extra.h"

typedef struct { int64_t numerator, denominator; } Rational;
enum { REACHABLE = 10000 };

static Rational rational(int64_t n, int64_t d) {
    if (d < 0) { n = -n; d = -d; }
    uint64_t g = euler_gcd((uint64_t)(n < 0 ? -n : n), (uint64_t)d);
    return (Rational){n / (int64_t)g, d / (int64_t)g};
}
static void expressions(const Rational *values, unsigned count, bool *reached) {
    if (count == 1) {
        Rational x = values[0];
        if (x.denominator == 1 && x.numerator > 0 && x.numerator < REACHABLE)
            reached[x.numerator] = true;
        return;
    }
    for (unsigned i = 0; i < count; ++i) {
        for (unsigned j = i + 1; j < count; ++j) {
            Rational next[4], options[6], a = values[i], b = values[j];
            unsigned kept = 0, n_options = 0;
            for (unsigned k = 0; k < count; ++k)
                if (k != i && k != j) next[kept++] = values[k];
            int64_t denominator = a.denominator * b.denominator;
            int64_t left = a.numerator * b.denominator;
            int64_t right = b.numerator * a.denominator;
            options[n_options++] = rational(left + right, denominator);
            options[n_options++] = rational(left - right, denominator);
            options[n_options++] = rational(right - left, denominator);
            options[n_options++] = rational(a.numerator * b.numerator, denominator);
            if (b.numerator)
                options[n_options++] = rational(a.numerator * b.denominator,
                                                a.denominator * b.numerator);
            if (a.numerator)
                options[n_options++] = rational(b.numerator * a.denominator,
                                                b.denominator * a.numerator);
            for (unsigned k = 0; k < n_options; ++k) {
                next[kept] = options[k];
                expressions(next, count - 1, reached);
            }
        }
    }
}
static unsigned consecutive(unsigned a, unsigned b, unsigned c, unsigned d) {
    bool reached[REACHABLE] = {false};
    Rational values[4] = {{a, 1}, {b, 1}, {c, 1}, {d, 1}};
    expressions(values, 4, reached);
    unsigned n = 1;
    while (n < REACHABLE && reached[n]) ++n;
    if (n == REACHABLE) euler_fail("Increase the consecutive-target storage");
    return n - 1;
}
int main(void) {
    unsigned longest = 0, answer = 0;
    for (unsigned a = 0; a <= 6; ++a)
        for (unsigned b = a + 1; b <= 7; ++b)
            for (unsigned c = b + 1; c <= 8; ++c)
                for (unsigned d = c + 1; d <= 9; ++d) {
                    unsigned length = consecutive(a, b, c, d);
                    if (length > longest) {
                        longest = length;
                        answer = 1000 * a + 100 * b + 10 * c + d;
                    }
                }
    printf("%04u\n", answer);
    return 0;
}
