/* Independent small cases for the new Euler 61..90 implementations.
 * Build from the repository root with:
 * gcc -O2 -std=c11 -Wall -Wextra -Wpedantic -Werror -Isupport/c
 *     tests/c_middle_examples.c -lgmp -lm -o build/tests/c_middle_examples
 */
#include <assert.h>
#define main p64_program
#include "../solutions/c/odd_period_square_roots_64.c"
#undef main
#define main p66_program
#include "../solutions/c/diophantine_equation_66.c"
#undef main
#define main p67_program
#include "../solutions/c/maximum_path_sum_ii_67.c"
#undef main
#define main p71_program
#include "../solutions/c/ordered_fractions_71.c"
#undef main
#define main p72_program
#include "../solutions/c/counting_fractions_72.c"
#undef main
#define main p73_program
#include "../solutions/c/counting_fractions_in_a_range_73.c"
#undef main
#define main p74_program
#include "../solutions/c/digit_factorial_chains_74.c"
#undef main
#define main p75_program
#include "../solutions/c/singular_integer_right_triangles_75.c"
#undef main
#define main p78_program
#include "../solutions/c/coin_partitions_78.c"
#undef main
#define main p81_program
#include "../solutions/c/path_sum_two_ways_81.c"
#undef main
#define main p82_program
#include "../solutions/c/path_sum_three_ways_82.c"
#undef main
#define main p83_program
#include "../solutions/c/path_sum_four_ways_83.c"
#undef main
#define main p84_program
#include "../solutions/c/monopoly_odds_84.c"
#undef main
#define main p86_program
#include "../solutions/c/cuboid_route_86.c"
#undef main
#define main p88_program
#include "../solutions/c/product_sum_numbers_88.c"
#undef main
#define main p89_program
#include "../solutions/c/roman_numerals_89.c"
#undef main
#define main p90_program
#include "../solutions/c/cube_digit_pairs_90.c"
#undef main

static void check_fractions(void) {
    for (unsigned limit = 2; limit <= 80; ++limit) {
        unsigned all = 0, between = 0;
        for (unsigned denominator = 2; denominator <= limit; ++denominator)
            for (unsigned numerator = 1; numerator < denominator; ++numerator) {
                if (euler_gcd(numerator, denominator) != 1) continue;
                ++all;
                if (3 * numerator > denominator && 2 * numerator < denominator) ++between;
            }
        assert(p72_count(limit) == all);
        assert(p73_between(3, 2, limit) == between);
    }
    assert(p71_numerator(8) == 2);
    assert(p71_numerator(1) == 0);
}

static void check_chains(void) {
    unsigned histogram[100] = {0};
    for (unsigned start = 1; start < 400; ++start) {
        unsigned visited[100], count = 0, n = start;
        for (;;) {
            unsigned i;
            for (i = 0; i < count; ++i) if (visited[i] == n) break;
            if (i != count) break;
            assert(count < 100); visited[count++] = n;
            /* Independent factorial calculation, without the solver's table. */
            unsigned sum = 0;
            do {
                unsigned fact = 1;
                for (unsigned d = 2; d <= n % 10; ++d) fact *= d;
                sum += fact; n /= 10;
            } while (n);
            n = sum;
        }
        ++histogram[count];
    }
    assert(p74_count(400, 1) == histogram[1]);
    assert(p74_count(400, 5) == histogram[5]);
    assert(p74_count(400, 10) == histogram[10]);
    assert(p74_count(400, 60) == histogram[60]);
}

static void check_triangles(void) {
    unsigned counts[301] = {0};
    for (unsigned a = 1; a <= 100; ++a) for (unsigned b = a; b <= 150; ++b)
        for (unsigned c = b + 1; a + b + c <= 300; ++c)
            if (a * a + b * b == c * c) ++counts[a + b + c];
    unsigned expected = 0;
    for (unsigned p = 1; p <= 300; ++p) expected += counts[p] == 1;
    assert(p75_unique(300) == expected);
}

static void check_partitions(void) {
    uint64_t ways[201] = {1};
    for (unsigned part = 1; part <= 200; ++part)
        for (unsigned n = part; n <= 200; ++n) ways[n] += ways[n - part];
    for (unsigned modulus = 2; modulus <= 20; ++modulus) {
        unsigned first = 1;
        while (first <= 200 && ways[first] % modulus) ++first;
        assert(first <= 200);
        assert(p78_first_divisible(modulus) == first);
    }
}

static void check_monopoly(void) {
    int chest[16], chance[16];
    for (unsigned i = 0; i < 16; ++i) chest[i] = chance[i] = P84_STAY;
    chance[0] = P84_BACK; chest[0] = 10;
    /* CH3 -> back three -> CC3 -> jail; both deck pointers advance. */
    assert(p84_transition(p84_index(28, 1, 0, 0), (P84Outcome){8, 0, 1}, chest, chance)
           == p84_index(10, 0, 1, 1));
    /* Three doubles send the player to jail without touching either deck. */
    assert(p84_transition(p84_index(35, 2, 4, 7), (P84Outcome){4, 1, 1}, chest, chance)
           == p84_index(10, 0, 4, 7));
    /* Merely landing on square 10 is not an instruction to go to jail. */
    assert(p84_transition(p84_index(6, 0, 4, 7), (P84Outcome){4, 1, 1}, chest, chance)
           == p84_index(10, 1, 4, 7));
    double marginal[40];
    assert(p84_modal(4, 1, marginal) == 101524);
    double mass = 0;
    for (unsigned i = 0; i < 40; ++i) { assert(marginal[i] >= 0); mass += marginal[i]; }
    assert(fabs(mass - 1) < 1e-11);
    assert(marginal[30] == 0);
    /* The statement's six-sided example matches the second ordering. It is
     * not an invariant of a fixed shuffle: ordering zero places 19 just
     * above GO. Both orderings agree on the requested four-sided answer. */
    assert(p84_modal(6, 0, marginal) == 102419);
    assert(p84_modal(6, 1, marginal) == 102400);
}

int main(void) {
    assert(p64_period(2) == 1 && p64_period(3) == 2 && p64_period(7) == 4);
    assert(p64_period(13) == 5 && p64_period(64) == 0);
    mpz_t pell;
    mpz_init(pell);
    p66_minimum_x(13, pell); assert(mpz_cmp_ui(pell, 649) == 0);
    p66_minimum_x(61, pell); assert(mpz_cmp_ui(pell, 1766319049) == 0);
    mpz_clear(pell);
    unsigned triangle[] = {3, 7, 4, 2, 4, 6, 8, 5, 9, 3};
    assert(p67_maximum(triangle, 4) == 23);
    check_fractions(); check_chains(); check_triangles(); check_partitions();
    static const unsigned matrix[] = {
        131, 673, 234, 103, 18, 201, 96, 342, 965, 150, 630, 803, 746, 422, 111,
        537, 699, 497, 121, 956, 805, 732, 524, 37, 331
    };
    assert(p81_minimum(matrix, 5) == 2427);
    assert(p82_minimum(matrix, 5) == 994);
    assert(p83_minimum(matrix, 5) == 2297);
    unsigned single[] = {123};
    assert(p81_minimum(single, 1) == 123 && p82_minimum(single, 1) == 123 && p83_minimum(single, 1) == 123);
    uint64_t cuboids = 0;
    for (unsigned side = 1; side <= 99; ++side) cuboids += p86_layer(side);
    assert(cuboids == 1975 && cuboids + p86_layer(100) == 2060);
    assert(p88_sum(6) == 30 && p88_sum(12) == 61);
    assert(p89_value("VIIII") == 9 && p89_minimum_length(9) == 2);
    assert(p89_value("MCMXLIV") == 1944 && p89_minimum_length(1944) == 7);
    unsigned left = (1u << 0) | (1u << 5) | (1u << 6) | (1u << 7) | (1u << 8) | (1u << 9);
    unsigned right = (1u << 1) | (1u << 2) | (1u << 3) | (1u << 4) | (1u << 6) | (1u << 7);
    assert(p90_valid(left, right));
    assert(!p90_valid(63, 63));
    check_monopoly();
    puts("Euler 61..90 independent examples passed");
    return 0;
}
