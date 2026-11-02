/* Project Euler 59: https://projecteuler.net/problem=59
 * Exhaust all 26^3 lowercase keys. The problem identifies the plaintext
 * as English, so rank readable candidates by letter frequencies and
 * common whole words. This language score is a heuristic, not a proof
 * that arbitrary English ciphertext always has a unique interpretation. */
#include "euler_extra.h"

enum { CIPHER_BYTES = 1455 };

static long english_score(const unsigned char *cipher, size_t length,
                          const unsigned char key[3], char *text) {
    static const int weights[26] = {
        8,2,3,4,13,2,2,6,7,1,1,4,2,7,8,2,1,6,6,9,3,1,2,1,2,1
    };
    long score = 0;
    for (size_t i = 0; i < length; ++i) {
        unsigned ch = cipher[i] ^ key[i % 3];
        if (ch < 32 || ch > 126) return LONG_MIN;
        text[i] = (char)ch;
        if (ch == ' ') score += 15;
        else if (isalpha((unsigned char)ch)) score += weights[tolower((unsigned char)ch) - 'a'];
        else if (strchr(".,;:'\"!?()-[]", (int)ch)) score -= 1;
        else if (isdigit((unsigned char)ch)) score -= 3;
        else score -= 12;
    }
    text[length] = '\0';
    static const char *words[] = {" the ", " and ", " of ", " to ", " in ",
                                  " that ", " is ", " a ", " for ", " with "};
    unsigned matches = 0;
    for (size_t i = 0; i < sizeof words / sizeof *words; ++i) {
        const char *found = text;
        while ((found = strstr(found, words[i])) != NULL) {
            score += 100;
            ++matches;
            ++found;
        }
    }
    return matches >= 3 ? score : LONG_MIN;
}

int main(void) {
    FILE *file = euler_open("tests/data/p059_cipher.txt");
    unsigned char cipher[CIPHER_BYTES];
    size_t length = 0;
    for (;;) {
        unsigned value;
        if (fscanf(file, "%u", &value) != 1 || value > 255 || length == CIPHER_BYTES)
            euler_fail("Invalid or truncated XOR ciphertext");
        cipher[length++] = (unsigned char)value;
        int ch;
        do { ch = fgetc(file); } while (ch != EOF && isspace((unsigned char)ch));
        if (ch == EOF) break;
        if (ch != ',') euler_fail("Expected comma in XOR ciphertext");
    }
    if (ferror(file) || length != CIPHER_BYTES) euler_fail("Expected 1455 XOR ciphertext bytes");
    fclose(file);
    char text[CIPHER_BYTES + 1], best_text[CIPHER_BYTES + 1];
    long best = LONG_MIN;
    bool tied = false;
    unsigned char key[3];
    for (key[0] = 'a'; key[0] <= 'z'; ++key[0])
        for (key[1] = 'a'; key[1] <= 'z'; ++key[1])
            for (key[2] = 'a'; key[2] <= 'z'; ++key[2]) {
                long score = english_score(cipher, length, key, text);
                if (score == LONG_MIN) continue;
                if (score > best) {
                    best = score;
                    memcpy(best_text, text, length + 1);
                    tied = false;
                } else if (score == best) tied = true;
            }
    if (best == LONG_MIN || tied) euler_fail("English plaintext could not be uniquely ranked");
    unsigned sum = 0;
    for (size_t i = 0; i < length; ++i) sum += (unsigned char)best_text[i];
    printf("%u\n", sum);
    return 0;
}
