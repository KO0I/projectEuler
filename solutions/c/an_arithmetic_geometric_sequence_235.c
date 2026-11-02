#include <stdio.h>

/*
 * Project Euler 235: An Arithmetic Geometric Sequence
 *
 * We need the unique r>1 for which
 *
 *   s(5000) = sum_{k=1..5000} (900-3k) r^(k-1) = -600000000000.
 *
 * On the narrow interval containing the answer the sum is continuous and
 * strictly decreasing: the many large-index negative terms dominate more as
 * r grows.  Bisection is therefore simpler and numerically safer than Newton's
 * method (no derivative or delicate starting estimate).  Each evaluation
 * builds successive powers by multiplication, avoiding a library pow call in
 * every term.  Two hundred halvings vastly exceed the precision of double;
 * the excess iterations make rounding to 12 decimal places unambiguous.
 */

static double sequence_sum(double r) {
    double power = 1.0;
    double sum = 0.0;
    for (int k = 1; k <= 5000; ++k) {
        sum += (900.0 - 3.0 * k) * power;
        power *= r;
    }
    return sum;
}

int main(void) {
    const double target = -600000000000.0;
    double low = 1.0;
    double high = 1.01; /* brackets the root while keeping all powers finite */

    for (int iteration = 0; iteration < 200; ++iteration) {
        const double middle = (low + high) / 2.0;
        if (sequence_sum(middle) > target)
            low = middle;
        else
            high = middle;
    }

    printf("%.12f\n", (low + high) / 2.0);
    return 0;
}
