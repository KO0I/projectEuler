/* Project Euler 151: A Preference for A5
 *
 * A state is the count of A2, A3, A4, and A5 sheets in the envelope.  Drawing
 * an Ai removes it and cutting the unused pieces adds one sheet of every
 * smaller size.  Each transition is weighted by count[i]/total.
 *
 * We add one to the expectation exactly when the envelope contains a single
 * sheet, excluding the final lone A5 as required.  The process is acyclic
 * despite occasionally increasing the sheet count, and the reachable count
 * bounds are [2][3][5][9]; memoization evaluates every state once. */
#include "euler_extra.h"
static double memo151[2][3][5][9];
static bool seen151[2][3][5][9];

static double expected151(int a, int b, int c, int d) {
    if (a + b + c + d == 0) return 0;
    if (seen151[a][b][c][d]) return memo151[a][b][c][d];
    seen151[a][b][c][d] = true;
    int sheets[4] = {a, b, c, d};
    int total = a + b + c + d;
    double result = total == 1 && d == 0 ? 1.0 : 0.0;
    for (int i = 0; i < 4; ++i) {
        if (!sheets[i]) continue;
        int next[4] = {a, b, c, d};
        --next[i];
        for (int j = i + 1; j < 4; ++j) ++next[j];
        result += (double)sheets[i] / total *
                  expected151(next[0], next[1], next[2], next[3]);
    }
    return memo151[a][b][c][d] = result;
}

int main(void) {
    printf("%.6f\n", expected151(1, 1, 1, 1));
    return 0;
}
