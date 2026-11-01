/* Project Euler 104: https://projecteuler.net/problem=104
 * Keep trailing digits modulo 10^9. For every pandigital tail, compute
 * the entire Fibonacci integer with GMP and inspect its leading digits.
 * No logarithmic approximation can reject a valid leading-digit match. */
#include "euler_extra.h"

static bool pandigital_number(unsigned n) {
    unsigned mask = 0;
    for (unsigned i = 0; i < 9; ++i) {
        unsigned digit = n % 10;
        if (!digit || (mask & (1U << digit))) return false;
        mask |= 1U << digit; n /= 10;
    }
    return n == 0 && mask == 0x3fe;
}
int main(void) {
    unsigned a = 1, b = 1;
    mpz_t value;
    mpz_init(value);
    for (unsigned long index = 3; ; ++index) {
        unsigned next = (a + b) % 1000000000U;
        a = b; b = next;
        if (!pandigital_number(b)) continue;
        mpz_fib_ui(value, index);
        char *text = mpz_get_str(NULL, 10, value);
        if (!text) euler_fail("Fibonacci decimal allocation failed");
        unsigned prefix = 0;
        bool long_enough = strlen(text) >= 9;
        if (long_enough)
            for (unsigned j = 0; j < 9; ++j) prefix = 10 * prefix + (text[j] - '0');
        free(text);
        if (long_enough && pandigital_number(prefix)) {
            printf("%lu\n", index);
            mpz_clear(value);
            return 0;
        }
    }
}
