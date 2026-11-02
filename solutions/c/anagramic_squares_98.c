/* Project Euler 98: https://projecteuler.net/problem=98
 * For each anagram pair, enumerate every square of that word length.
 * Its digits determine the only possible letter mapping. Apply that
 * bijection to the other word and test the result with an exact sqrt. */
#include "euler_extra.h"

typedef struct { char text[32]; unsigned char counts[26]; size_t length; } Word;
static uint64_t pair_maximum(const Word *a, const Word *b) {
    if (a->length >= 19) euler_fail("Anagram pair exceeds uint64_t square range");
    uint64_t upper = 1;
    for (size_t i = 0; i < a->length; ++i) upper *= 10;
    uint64_t lower = upper / 10, first = euler_isqrt(lower);
    if (first * first < lower) ++first;
    uint64_t last = euler_isqrt(upper - 1), maximum = 0;
    for (uint64_t root = first; root <= last; ++root) {
        uint64_t square = root * root;
        char digits[32];
        snprintf(digits, sizeof digits, "%" PRIu64, square);
        int letter[26], used[10];
        for (unsigned i = 0; i < 26; ++i) letter[i] = -1;
        for (unsigned i = 0; i < 10; ++i) used[i] = -1;
        bool consistent = true;
        for (size_t i = 0; i < a->length; ++i) {
            unsigned c = a->text[i] - 'A', digit = digits[i] - '0';
            if ((letter[c] != -1 && letter[c] != (int)digit) ||
                (used[digit] != -1 && used[digit] != (int)c)) {
                consistent = false; break;
            }
            letter[c] = (int)digit; used[digit] = (int)c;
        }
        if (!consistent || letter[b->text[0] - 'A'] == 0) continue;
        uint64_t other = 0;
        for (size_t i = 0; i < b->length; ++i)
            other = 10 * other + (unsigned)letter[b->text[i] - 'A'];
        uint64_t other_root = euler_isqrt(other);
        if (other_root * other_root == other) {
            if (square > maximum) maximum = square;
            if (other > maximum) maximum = other;
        }
    }
    return maximum;
}
int main(void) {
    enum { WORDS = 1786 };
    Word *words = euler_calloc(WORDS, sizeof *words);
    FILE *file = euler_open("tests/data/p098_words.txt");
    size_t count = 0;
    for (;;) {
        int ch;
        do { ch = fgetc(file); } while (ch != EOF && isspace((unsigned char)ch));
        if (ch != '"' || count == WORDS) euler_fail("Invalid anagram word input");
        Word *word = &words[count++];
        while ((ch = fgetc(file)) != '"') {
            if (ch < 'A' || ch > 'Z' || word->length + 1 >= sizeof word->text)
                euler_fail("Invalid anagram word");
            word->text[word->length++] = (char)ch;
            ++word->counts[ch - 'A'];
        }
        if (!word->length) euler_fail("Empty anagram word");
        do { ch = fgetc(file); } while (ch != EOF && isspace((unsigned char)ch));
        if (ch == EOF) break;
        if (ch != ',') euler_fail("Invalid anagram word separator");
    }
    if (count != WORDS || ferror(file)) euler_fail("Incomplete anagram word input");
    fclose(file);
    uint64_t maximum = 0;
    for (size_t i = 0; i < count; ++i)
        for (size_t j = i + 1; j < count; ++j) {
            if (words[i].length != words[j].length ||
                memcmp(words[i].counts, words[j].counts, sizeof words[i].counts)) continue;
            uint64_t result = pair_maximum(&words[i], &words[j]);
            if (result > maximum) maximum = result;
        }
    free(words);
    printf("%" PRIu64 "\n", maximum);
    return 0;
}
