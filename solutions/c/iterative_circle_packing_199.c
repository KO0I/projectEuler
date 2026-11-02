#include <math.h>
#include <stdio.h>

/*
 * Project Euler 199: iterative circle packing.
 *
 * Work with signed curvatures k=1/r.  The enclosing unit circle has k=-1;
 * the three congruent starting circles have radius 2*sqrt(3)-3, obtained by
 * comparing the circumradius of their equilateral triangle of centres with
 * 1-r.  Descartes' circle theorem gives the positive curvature inserted into
 * a curvilinear triangular gap:
 *
 * k4 = k1+k2+k3 + 2*sqrt(k1*k2+k2*k3+k3*k1).
 *
 * Inserting that circle splits one gap into the three gaps formed by k4 and
 * each pair of old boundaries.  The recursion follows this construction for
 * ten iterations.  Since the outer radius is one, each circle occupies area
 * fraction r^2=1/k^2; pi cancels from the final uncovered fraction.
 */

static double filled_fraction(double k1, double k2, double k3, int depth) {
    if (depth == 0) {
        return 0.0;
    }
    const double k4 = k1 + k2 + k3 +
                      2.0 * sqrt(k1 * k2 + k2 * k3 + k3 * k1);
    return 1.0 / (k4 * k4) +
           filled_fraction(k1, k2, k4, depth - 1) +
           filled_fraction(k2, k3, k4, depth - 1) +
           filled_fraction(k3, k1, k4, depth - 1);
}

int main(void) {
    const double inner_radius = 2.0 * sqrt(3.0) - 3.0;
    const double inner_curvature = 1.0 / inner_radius;

    double covered = 3.0 * inner_radius * inner_radius;
    /* One central gap has three inner boundaries; the other three gaps each
       touch the enclosing circle and two of the congruent inner circles. */
    covered += filled_fraction(inner_curvature, inner_curvature, inner_curvature, 10);
    covered += 3.0 * filled_fraction(-1.0, inner_curvature, inner_curvature, 10);
    printf("%.8f\n", 1.0 - covered);
    return 0;
}
