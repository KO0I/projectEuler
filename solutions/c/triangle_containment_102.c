/* Project Euler 102: https://projecteuler.net/problem=102
 * The origin lies strictly inside a triangle exactly when the cross
 * products of consecutive position vectors all have the same sign. */
#include "euler_extra.h"
static bool contains_origin(const long long p[6]) {
    long long a = p[0] * p[3] - p[1] * p[2];
    long long b = p[2] * p[5] - p[3] * p[4];
    long long c = p[4] * p[1] - p[5] * p[0];
    return (a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0);
}
int main(void) {
    FILE *file = euler_open("tests/data/p102_triangles.txt");
    char line[256];
    unsigned total = 0, lines = 0;
    while (fgets(line, sizeof line, file)) {
        long long p[6];
        char *cursor = line;
        for (unsigned i = 0; i < 6; ++i) {
            char *end;
            errno = 0;
            p[i] = strtoll(cursor, &end, 10);
            if (cursor == end || errno || p[i] < -1000 || p[i] > 1000)
                euler_fail("Invalid triangle coordinate");
            while (isspace((unsigned char)*end)) ++end;
            if (i < 5) {
                if (*end != ',') euler_fail("Invalid triangle separator");
                cursor = end + 1;
            } else if (*end) euler_fail("Extra triangle coordinate");
        }
        total += contains_origin(p); ++lines;
    }
    if (lines != 1000 || ferror(file)) euler_fail("Incomplete triangle input");
    fclose(file);
    printf("%u\n", total);
    return 0;
}
