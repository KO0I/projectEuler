#include <math.h>
#include <stdio.h>

/*
 * Project Euler 226: A Scoop of Blancmange.
 *
 * Evaluate the Takagi/blancmange function directly from its rapidly decaying
 * series.  The required boundary is the lower semicircle
 *
 *   C(x)=1/2-sqrt(1/16-(x-1/4)^2).
 *
 * Bisection locates their nontrivial intersection in (0,1/4); the other is
 * x=1/2.  The enclosed area is integral(B-C) between those roots.  Composite
 * Simpson integration with one million subintervals resolves the dyadic
 * corners of the fractal well beyond the requested eight decimal places.
 * Terms after n=52 are below double precision and are intentionally omitted.
 */

static double blancmange(double x) {
    double sum = 0.0;
    double weight = 1.0;
    double scaled = x;
    for (int n = 0; n < 53; ++n) {
        const double nearest = floor(scaled + 0.5);
        sum += fabs(scaled - nearest) * weight;
        scaled *= 2.0;
        weight *= 0.5;
    }
    return sum;
}

static double lower_circle(double x) {
    const double dx = x - 0.25;
    return 0.5 - sqrt(0.0625 - dx * dx);
}

static double difference(double x) {
    return blancmange(x) - lower_circle(x);
}

int main(void) {
    double low = 0.0;
    double high = 0.25;
    for (int iteration = 0; iteration < 100; ++iteration) {
        const double middle = (low + high) / 2.0;
        if (difference(middle) > 0.0) {
            high = middle;
        } else {
            low = middle;
        }
    }
    const double from = (low + high) / 2.0;
    const double to = 0.5;
    enum { INTERVALS = 1000000 };
    const double step = (to - from) / INTERVALS;
    double weighted_sum = difference(from) + difference(to);
    for (int i = 1; i < INTERVALS; ++i) {
        weighted_sum += (i & 1 ? 4.0 : 2.0) * difference(from + i * step);
    }
    printf("%.8f\n", weighted_sum * step / 3.0);
    return 0;
}
