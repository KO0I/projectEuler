#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 236: Luxury Hampers
 *
 * Write a_i and b_i for spoiled units and m=u/v in lowest terms.  Equality
 * of the per-product ratios gives the integer equation
 *
 *     v*b_i*A_i = u*a_i*B_i.
 *
 * If g_i=gcd(u B_i, v A_i), every positive solution is
 *
 *     a_i=(v A_i/g_i) k_i,   b_i=(u B_i/g_i) k_i,
 *
 * with a short, explicit range for k_i imposed by the supplied quantities.
 * Substitution into the reversed overall-rate equation leaves one linear
 * Diophantine equation in the five k_i values.
 *
 * Products 2, 3, and 5 all have A_i/B_i=41/59.  Their reduced spoiled ratio
 * b_i/a_i=r/s therefore determines m=41r/(59s), and their three coefficients
 * in the linear equation are identical.  Only their sum T=k2+k3+k5 matters;
 * because each k occupies a contiguous positive interval, every T from 3 to
 * K2+K3+K5 is attainable.  We consequently enumerate coprime (r,s), then
 * only the small k1/k4 rectangle and solve for T instead of searching five
 * nested loops.  Counting 35 feasible ratios also checks the fact supplied in
 * the problem statement rather than merely printing a preselected fraction.
 */

static const int supplied_a[5] = {5248, 1312, 2624, 5760, 3936};
static const int supplied_b[5] = {640, 1888, 3776, 3776, 5664};
enum { TOTAL_A = 18880, TOTAL_B = 15744 };

static int64_t gcd64(int64_t a, int64_t b) {
    while (b != 0) {
        const int64_t remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

int main(void) {
    int valid_count = 0;
    int64_t best_u = 0;
    int64_t best_v = 1;

    /* r and s need not exceed the smallest B and A supplies in the shared
       group: k_i is positive, so larger base spoilage is impossible. */
    for (int s = 1; s <= supplied_a[1]; ++s) {
        for (int r = 1; r <= supplied_b[1]; ++r) {
            if (gcd64(r, s) != 1 || (int64_t)41 * r <= (int64_t)59 * s)
                continue; /* m must be greater than one */

            int64_t u = (int64_t)41 * r;
            int64_t v = (int64_t)59 * s;
            const int64_t reduction = gcd64(u, v);
            u /= reduction;
            v /= reduction;

            int64_t coefficient[5];
            int max_k[5];
            int feasible_bases = 1;
            for (int i = 0; i < 5; ++i) {
                const int64_t g = gcd64(u * supplied_b[i],
                                        v * supplied_a[i]);
                const int64_t base_a = v * supplied_a[i] / g;
                const int64_t base_b = u * supplied_b[i] / g;
                int64_t ka = supplied_a[i] / base_a;
                int64_t kb = supplied_b[i] / base_b;
                max_k[i] = (int)(ka < kb ? ka : kb);
                if (max_k[i] < 1)
                    feasible_bases = 0;

                /* TOTAL_B*v*sum(a) = TOTAL_A*u*sum(b). */
                coefficient[i] = (int64_t)TOTAL_B * v * base_a
                                 - (int64_t)TOTAL_A * u * base_b;
            }
            if (!feasible_bases)
                continue;

            const int min_t = 3;
            const int max_t = max_k[1] + max_k[2] + max_k[4];
            const int64_t common = coefficient[1];
            int found = 0;
            for (int k1 = 1; k1 <= max_k[0] && !found; ++k1) {
                for (int k4 = 1; k4 <= max_k[3]; ++k4) {
                    const int64_t remainder =
                        -(coefficient[0] * k1 + coefficient[3] * k4);
                    if (common == 0) {
                        if (remainder == 0)
                            found = 1;
                    } else if (remainder % common == 0) {
                        const int64_t t = remainder / common;
                        if (t >= min_t && t <= max_t)
                            found = 1;
                    }
                    if (found)
                        break;
                }
            }

            if (found) {
                ++valid_count;
                if (u * best_v > best_u * v) {
                    best_u = u;
                    best_v = v;
                }
            }
        }
    }

    /* The count is an independent guard on the enumeration boundaries. */
    if (valid_count != 35)
        return 1;
    printf("%lld/%lld\n", (long long)best_u, (long long)best_v);
    return 0;
}
