/* Project Euler 42: https://projecteuler.net/problem=42
 * Read the official quoted words, sum their A=1,...,Z=26 scores, and
 * recognize triangular scores by the exact square test 8s+1=(2n+1)^2. */
#include "euler_extra.h"

static int next_nonspace(FILE *file) {
    int ch;
    do { ch = fgetc(file); } while (ch != EOF && isspace((unsigned char)ch));
    return ch;
}

int main(void) {
    FILE *file = euler_open("tests/data/p042_words.txt");
    unsigned words = 0, triangles = 0;
    int ch = next_nonspace(file);
    for (;;) {
        if (ch != '"') euler_fail("Expected a quoted word in p042_words.txt");
        unsigned score = 0, length = 0;
        while ((ch = fgetc(file)) != EOF && ch != '"') {
            if (ch < 'A' || ch > 'Z' || ++length > 64)
                euler_fail("Invalid word in p042_words.txt");
            score += (unsigned)(ch - 'A' + 1);
        }
        if (ch != '"' || !length) euler_fail("Truncated or empty word in p042_words.txt");
        uint64_t discriminant = 8 * (uint64_t)score + 1;
        uint64_t root = euler_isqrt(discriminant);
        triangles += root * root == discriminant;
        ++words;
        ch = next_nonspace(file);
        if (ch == EOF) break;
        if (ch != ',') euler_fail("Expected comma in p042_words.txt");
        ch = next_nonspace(file);
    }
    if (ferror(file) || words != 1786) euler_fail("Expected 1786 words in p042_words.txt");
    fclose(file);
    printf("%u\n", triangles);
    return 0;
}
