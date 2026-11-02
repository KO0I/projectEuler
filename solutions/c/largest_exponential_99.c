/* Project Euler 99: https://projecteuler.net/problem=99
 * Compare exponent*log(base) using exact rational intervals. After binary
 * range reduction, z=(t-1)/(t+1) <= 1/3, and log(t)=2*atanh(z).
 * The omitted series tail is bounded by its first term divided by
 * 1-z*z. If intervals overlap, compare the full integers with GMP. */
#include "euler_extra.h"

static void logarithm_series(mpq_t low, mpq_t high, unsigned long num,
                             unsigned long den) {
    enum { TERMS = 24 };
    mpq_t z, z2, power, term, divisor, gap;
    mpq_inits(z, z2, power, term, divisor, gap, NULL);
    mpq_set_ui(z, num, den); mpq_canonicalize(z);
    mpq_mul(z2, z, z); mpq_set(power, z); mpq_set_ui(low, 0, 1);
    for (unsigned j = 0; j < TERMS; ++j) {
        mpq_set_ui(divisor, 2 * j + 1, 1);
        mpq_div(term, power, divisor); mpq_add(low, low, term);
        mpq_mul(power, power, z2);
    }
    mpq_mul_2exp(low, low, 1);
    mpq_set_ui(gap, 1, 1); mpq_sub(gap, gap, z2);
    mpq_set_ui(divisor, 2 * TERMS + 1, 1); mpq_mul(divisor, divisor, gap);
    mpq_div(term, power, divisor); mpq_mul_2exp(term, term, 1);
    mpq_add(high, low, term);
    mpq_clears(z, z2, power, term, divisor, gap, NULL);
}
static void power_log_interval(mpq_t low, mpq_t high, unsigned long base,
                               unsigned long exponent, const mpq_t log2_low,
                               const mpq_t log2_high) {
    unsigned long power2 = 1, k = 0;
    while (power2 <= base / 2) { power2 *= 2; ++k; }
    logarithm_series(low, high, base - power2, base + power2);
    mpq_t factor, temp;
    mpq_inits(factor, temp, NULL);
    mpq_set_ui(factor, k, 1);
    mpq_mul(temp, log2_low, factor); mpq_add(low, low, temp);
    mpq_mul(temp, log2_high, factor); mpq_add(high, high, temp);
    mpq_set_ui(factor, exponent, 1);
    mpq_mul(low, low, factor); mpq_mul(high, high, factor);
    mpq_clears(factor, temp, NULL);
}
int main(void) {
    FILE *file = euler_open("tests/data/p099_base_exp.txt");
    mpq_t low, high, best_low, best_high, log2_low, log2_high;
    mpq_inits(low, high, best_low, best_high, log2_low, log2_high, NULL);
    logarithm_series(log2_low, log2_high, 1, 3);
    unsigned best_line = 0, lines = 0;
    unsigned long best_base = 0, best_exponent = 0;
    char line[256];
    while (fgets(line, sizeof line, file)) {
        char *p = line, *end;
        while (isspace((unsigned char)*p)) ++p;
        if (!isdigit((unsigned char)*p)) euler_fail("Invalid exponential base");
        errno = 0;
        unsigned long base = strtoul(p, &end, 10);
        if (errno || !base || base > ULONG_MAX / 2 || *end != ',')
            euler_fail("Invalid base/exponent input");
        p = end + 1;
        while (isspace((unsigned char)*p)) ++p;
        if (!isdigit((unsigned char)*p)) euler_fail("Invalid exponential exponent");
        errno = 0;
        unsigned long exponent = strtoul(p, &end, 10);
        if (errno || !exponent) euler_fail("Invalid exponential exponent");
        while (isspace((unsigned char)*end)) ++end;
        if (*end) euler_fail("Extra base/exponent data");
        ++lines;
        power_log_interval(low, high, base, exponent, log2_low, log2_high);
        bool larger = best_line == 0 || mpq_cmp(low, best_high) > 0;
        if (best_line && !larger && mpq_cmp(high, best_low) >= 0) {
            mpz_t a, b;
            mpz_inits(a, b, NULL);
            mpz_ui_pow_ui(a, base, exponent);
            mpz_ui_pow_ui(b, best_base, best_exponent);
            larger = mpz_cmp(a, b) > 0;
            mpz_clears(a, b, NULL);
        }
        if (larger) {
            best_line = lines; best_base = base; best_exponent = exponent;
            mpq_set(best_low, low); mpq_set(best_high, high);
        }
    }
    if (lines != 1000 || ferror(file)) euler_fail("Incomplete base/exponent input");
    fclose(file);
    mpq_clears(low, high, best_low, best_high, log2_low, log2_high, NULL);
    printf("%u\n", best_line);
    return 0;
}
