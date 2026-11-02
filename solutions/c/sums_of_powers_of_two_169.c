/* Project Euler 169: Sums of Powers of Two
 *
 * A representation chooses coefficient 0, 1, or 2 for every power of two.
 * Process the binary digits of 10^25 from least significant upward.  The DP
 * state is the carry into the current bit; a coefficient is legal when
 * (coefficient+carry) has the required parity, and its half becomes the next
 * carry.
 *
 * GMP stores the input exactly, but the number of representations fits in
 * uint64_t.  When every input bit has been consumed, only zero carry denotes a
 * complete representation, so dp[0] is the answer. */
#include "euler_extra.h"
int main(void) {
    mpz_t n;
    mpz_init_set_ui(n, 10);
    mpz_pow_ui(n, n, 25);
    uint64_t dp[2] = {1, 0};
    uint64_t next[2];
    while (mpz_sgn(n) > 0) {
        unsigned bit = mpz_tstbit(n, 0);
        next[0] = next[1] = 0;
        for (unsigned carry = 0; carry < 2; ++carry) {
            for (unsigned digit = 0; digit <= 2; ++digit) {
                if (((digit + carry) & 1) == bit)
                    next[(digit + carry) >> 1] += dp[carry];
            }
        }
        dp[0] = next[0];
        dp[1] = next[1];
        mpz_fdiv_q_2exp(n, n, 1);
    }
    printf("%" PRIu64 "\n", dp[0]);
    mpz_clear(n);
    return 0;
}
