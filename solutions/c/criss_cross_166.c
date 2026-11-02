/* Project Euler 166: Criss Cross
 *
 * Name the 4x4 entries a..p.  Enumerating all sixteen digits would require
 * 10^16 cases.  Instead choose the first row a,b,c,d and three cells e,f,g of
 * the second row; their common line sum determines h.  Choosing j and k then
 * lets the row, column, and anti-diagonal equations determine i,l,m,n,o,p.
 *
 * Every derived value is rejected unless it is a decimal digit.  All line
 * equations except the main diagonal have then been enforced by construction,
 * so one final diagonal comparison completes the test.  This parametrization
 * visits each valid grid once and uses only small signed integers. */
#include "euler_extra.h"

static bool digit166(int value) { return value >= 0 && value <= 9; }

int main(void) {
    uint64_t answer = 0;
    for (int a = 0; a <= 9; ++a)
    for (int b = 0; b <= 9; ++b)
    for (int c = 0; c <= 9; ++c)
    for (int d = 0; d <= 9; ++d) {
        int sum = a + b + c + d;
        for (int e = 0; e <= 9; ++e)
        for (int f = 0; f <= 9; ++f)
        for (int g = 0; g <= 9; ++g) {
            int h = sum - e - f - g;
            if (!digit166(h)) continue;
            for (int j = 0; j <= 9; ++j) {
                int i = d + g + j - a - e;
                if (!digit166(i)) continue;
                for (int k = 0; k <= 9; ++k) {
                int l = sum - i - j - k;
                int m = sum - a - e - i;
                int n = sum - b - f - j;
                int o = sum - c - g - k;
                int p = sum - d - h - l;
                if (!digit166(l) || !digit166(m) || !digit166(n) ||
                    !digit166(o) || !digit166(p)) continue;
                if (a + f + k + p == sum) ++answer;
                }
            }
        }
    }
    printf("%" PRIu64 "\n", answer);
    return 0;
}
