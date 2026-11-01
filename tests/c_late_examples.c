/* Independent small cases and examples for the new Euler 91-120 solvers.
 * Compile from the root with -Isupport/c and link -lgmp -lm. */
#include <assert.h>
#define main euler91_main
#include "../solutions/c/right_triangles_with_integer_coordinates_91.c"
#undef main
#define main euler93_main
#include "../solutions/c/arithmetic_expressions_93.c"
#undef main
#define main euler99_main
#include "../solutions/c/largest_exponential_99.c"
#undef main
#define main euler102_main
#include "../solutions/c/triangle_containment_102.c"
#undef main
#define main euler105_main
#include "../solutions/c/special_subset_sums_testing_105.c"
#undef main
#define main euler106_main
#include "../solutions/c/special_subset_sums_meta_testing_106.c"
#undef main
#define main euler108_main
#include "../solutions/c/diophantine_reciprocals_i_108.c"
#undef main
#define main euler111_main
#include "../solutions/c/primes_with_runs_111.c"
#undef main
#define main euler113_main
#include "../solutions/c/non_bouncy_numbers_113.c"
#undef main
#define main euler114_main
#include "../solutions/c/counting_block_combinations_i_114.c"
#undef main
#define main euler116_main
#include "../solutions/c/red_green_or_blue_tiles_116.c"
#undef main
#define main euler117_main
#include "../solutions/c/red_green_and_blue_tiles_117.c"
#undef main
#define main euler120_main
#include "../solutions/c/square_remainders_120.c"
#undef main

static unsigned brute_triangles(unsigned limit) {
    unsigned count = 0, width = limit + 1;
    for (unsigned p = 1; p < width * width; ++p)
        for (unsigned q = p + 1; q < width * width; ++q) {
            int px = (int)(p % width), py = (int)(p / width);
            int qx = (int)(q % width), qy = (int)(q / width);
            int a = px * px + py * py, b = qx * qx + qy * qy;
            int c = (px - qx) * (px - qx) + (py - qy) * (py - qy);
            count += a + b == c || a + c == b || b + c == a;
        }
    return count;
}
static unsigned brute_blocks(unsigned length, unsigned minimum) {
    unsigned count = 0;
    for (unsigned mask = 0; mask < (1U << length); ++mask) {
        unsigned run = 0;
        bool valid = true;
        for (unsigned i = 0; i <= length; ++i) {
            if (i < length && (mask & (1U << i))) ++run;
            else { if (run && run < minimum) valid = false; run = 0; }
        }
        count += valid;
    }
    return count;
}
static unsigned brute_reciprocal(unsigned threshold) {
    for (unsigned n = 1; ; ++n) {
        unsigned count = 0;
        for (unsigned x = n + 1; x <= 2 * n; ++x)
            count += (n * x) % (x - n) == 0;
        if (count > threshold) return n;
    }
}
static uint64_t brute_remainder(unsigned a) {
    uint64_t left = 1, right = 1, best_remainder = 0, modulus = (uint64_t)a * a;
    for (unsigned n = 1; n <= 2 * a; ++n) {
        left = left * (a - 1) % modulus;
        right = right * (a + 1) % modulus;
        uint64_t value = (left + right) % modulus;
        if (value > best_remainder) best_remainder = value;
    }
    return best_remainder;
}
static void test_log_intervals(void) {
    mpq_t a_low, a_high, b_low, b_high, log2_low, log2_high;
    mpq_inits(a_low, a_high, b_low, b_high, log2_low, log2_high, NULL);
    logarithm_series(log2_low, log2_high, 1, 3);
    power_log_interval(a_low, a_high, 2, 11, log2_low, log2_high);
    power_log_interval(b_low, b_high, 3, 7, log2_low, log2_high);
    assert(mpq_cmp(a_high, b_low) < 0); /* 2048 < 2187. */
    power_log_interval(a_low, a_high, 2, 6, log2_low, log2_high);
    power_log_interval(b_low, b_high, 4, 3, log2_low, log2_high);
    assert(mpq_cmp(a_low, b_high) <= 0 && mpq_cmp(b_low, a_high) <= 0);
    mpq_clears(a_low, a_high, b_low, b_high, log2_low, log2_high, NULL);
}
int main(void) {
    for (unsigned n = 1; n <= 6; ++n) assert(triangles(n) == brute_triangles(n));
    assert(consecutive(1, 2, 3, 4) == 28);
    assert(consecutive(1, 2, 5, 8) == 51);
    Rational half = rational(-2, -4);
    assert(half.numerator == 1 && half.denominator == 2);
    test_log_intervals();
    const long long inside[] = {-340, 495, -153, -910, 835, -947};
    const long long outside[] = {-175, 41, -421, -714, 574, -645};
    const long long edge[] = {-1, 0, 1, 0, 0, 2};
    assert(contains_origin(inside)); assert(!contains_origin(outside));
    assert(!contains_origin(edge));
    uint64_t bad[] = {81, 88, 75, 42, 87, 84, 86, 65};
    uint64_t good[] = {157, 150, 164, 119, 79, 159, 161, 139, 158};
    assert(!special(bad, 8)); assert(special(good, 9));
    assert(comparisons(4) == 1); assert(comparisons(7) == 70);
    for (unsigned threshold = 1; threshold <= 30; ++threshold)
        assert(least_denominator(threshold) == brute_reciprocal(threshold));
    assert(least_denominator(100) == 1260);
    assert(run_sum(4) == 273700);
    Runs runs = {4, 1, 0, 0};
    replacements(&runs, 0, 1, 0);
    assert(runs.count == 9 && runs.sum == 22275);
    assert(non_bouncy(1) == 9); assert(non_bouncy(6) == 12951);
    assert(non_bouncy(10) == 277032);
    for (unsigned length = 0; length <= 12; ++length)
        for (unsigned minimum = 2; minimum <= 5; ++minimum)
            assert(block_ways(length, minimum) == brute_blocks(length, minimum));
    assert(coloured_ways(5, 2) == 7); assert(coloured_ways(5, 3) == 3);
    assert(coloured_ways(5, 4) == 2); assert(mixed_ways(5) == 15);
    assert(mixed_ways(0) == 1);
    for (unsigned a = 3; a <= 100; ++a) assert(maximum_remainder(a) == brute_remainder(a));
    puts("Late-group independent regression checks passed");
    return 0;
}
