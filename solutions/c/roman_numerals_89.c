/* https://projecteuler.net/problem=89
 * Decode each supplied Roman numeral by subtracting a symbol when the next
 * symbol is larger. Greedy canonical tokens M,CM,D,CD,...,IV,I give the
 * shortest valid form; only its length is needed to count saved characters. */
#include "euler_extra.h"

static unsigned p89_symbol(char symbol) {
    switch (symbol) {
        case 'I': return 1; case 'V': return 5; case 'X': return 10; case 'L': return 50;
        case 'C': return 100; case 'D': return 500; case 'M': return 1000;
        default: euler_fail("Invalid Roman symbol"); return 0;
    }
}

static unsigned p89_value(const char *text) {
    int value = 0;
    for (size_t i = 0; text[i]; ++i) {
        unsigned current = p89_symbol(text[i]);
        if (text[i + 1] && p89_symbol(text[i + 1]) > current) value -= (int)current;
        else value += (int)current;
    }
    if (value <= 0) euler_fail("Nonpositive Roman numeral");
    return (unsigned)value;
}

static unsigned p89_minimum_length(unsigned n) {
    static const unsigned values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    static const unsigned lengths[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1};
    unsigned length = 0;
    for (size_t i = 0; i < sizeof values / sizeof *values; ++i) {
        length += n / values[i] * lengths[i]; n %= values[i];
    }
    return length;
}

int main(void) {
    FILE *file = euler_open("tests/data/p089_roman.txt");
    char token[512]; unsigned count = 0, saved = 0;
    while (fscanf(file, "%511s", token) == 1) {
        unsigned before = (unsigned)strlen(token), after = p89_minimum_length(p89_value(token));
        if (after > before) euler_fail("Input is not a valid nonminimal Roman numeral");
        saved += before - after; ++count;
    }
    if (ferror(file) || count != 1000) euler_fail("Expected 1000 Roman numerals");
    fclose(file);
    printf("%u\n", saved);
    return 0;
}
