#include <stdio.h>
#include <string.h>

/*
 * Project Euler 185: Number Mind.
 *
 * This is a constraint search, not a permutation of 10^16 strings.  For each
 * clue we retain the number of matches already committed.  A trial digit is
 * impossible as soon as it would exceed one clue's target.  Conversely, a
 * node is impossible if an unfinished clue cannot obtain enough matches from
 * the positions still available to it.
 *
 * The clue with zero matches immediately removes one candidate from every
 * column.  At each node we recompute the legal digits of every unfilled
 * column and branch on the smallest domain (the standard MRV heuristic).
 * Clues whose quota has already been reached remove their matching digit;
 * this makes domains collapse quickly near the leaves.  Digits are tried in
 * descending order of how many still-needed clue matches they satisfy, which
 * affects speed only, never correctness.
 */

enum { CLUES = 22, WIDTH = 16 };

static const char guesses[CLUES][WIDTH + 1] = {
    "5616185650518293", "3847439647293047", "5855462940810587",
    "9742855507068353", "4296849643607543", "3174248439465858",
    "4513559094146117", "7890971548908067", "8157356344118483",
    "2615250744386899", "8690095851526254", "6375711915077050",
    "6913859173121360", "6442889055042768", "2321386104303845",
    "2326509471271448", "5251583379644322", "1748270476758276",
    "4895722652190306", "3041631117224635", "1841236454324589",
    "2659862637316867"
};

static const unsigned char targets[CLUES] = {
    2, 1, 3, 3, 3, 1, 2, 3, 1, 2, 3, 1, 1, 2, 0, 2, 2, 3, 1, 3, 3, 2
};

static char answer[WIDTH + 1];
static unsigned char matches[CLUES];
static unsigned char filled[WIDTH];

static int digit_is_legal(int position, int digit) {
    /* The zero clue is an especially cheap rejection and avoids scanning the
       remaining clues for one ninth of all candidate digits. */
    if (digit == guesses[14][position] - '0') {
        return 0;
    }
    for (int clue = 0; clue < CLUES; ++clue) {
        if (digit == guesses[clue][position] - '0' && matches[clue] == targets[clue]) {
            return 0;
        }
    }
    return 1;
}

static int remaining_capacity_is_sufficient(void) {
    for (int clue = 0; clue < CLUES; ++clue) {
        int possible = 0;
        for (int position = 0; position < WIDTH; ++position) {
            if (!filled[position]) {
                const int digit = guesses[clue][position] - '0';
                possible += digit_is_legal(position, digit);
            }
        }
        if ((int)matches[clue] + possible < targets[clue]) {
            return 0;
        }
    }
    return 1;
}

static int search(int depth) {
    if (depth == WIDTH) {
        for (int clue = 0; clue < CLUES; ++clue) {
            if (matches[clue] != targets[clue]) {
                return 0;
            }
        }
        answer[WIDTH] = '\0';
        return 1;
    }

    int best_position = -1;
    int best_digits[10];
    int best_scores[10];
    int best_count = 11;

    for (int position = 0; position < WIDTH; ++position) {
        if (filled[position]) {
            continue;
        }
        int digits[10];
        int scores[10];
        int count = 0;
        for (int digit = 0; digit <= 9; ++digit) {
            if (!digit_is_legal(position, digit)) {
                continue;
            }
            int score = 0;
            for (int clue = 0; clue < CLUES; ++clue) {
                if (matches[clue] < targets[clue] && digit == guesses[clue][position] - '0') {
                    ++score;
                }
            }
            digits[count] = digit;
            scores[count++] = score;
        }
        if (count == 0) {
            return 0;
        }
        if (count < best_count) {
            best_position = position;
            best_count = count;
            memcpy(best_digits, digits, (size_t)count * sizeof(*digits));
            memcpy(best_scores, scores, (size_t)count * sizeof(*scores));
        }
    }

    /* A tiny insertion sort gives the most constraining/value-producing
       candidates first, often finding the unique solution before exploring
       the low-information digits. */
    for (int i = 1; i < best_count; ++i) {
        const int digit = best_digits[i];
        const int score = best_scores[i];
        int j = i;
        while (j > 0 && best_scores[j - 1] < score) {
            best_digits[j] = best_digits[j - 1];
            best_scores[j] = best_scores[j - 1];
            --j;
        }
        best_digits[j] = digit;
        best_scores[j] = score;
    }

    filled[best_position] = 1;
    for (int i = 0; i < best_count; ++i) {
        const int digit = best_digits[i];
        answer[best_position] = (char)('0' + digit);
        for (int clue = 0; clue < CLUES; ++clue) {
            matches[clue] += (unsigned char)(digit == guesses[clue][best_position] - '0');
        }
        if (remaining_capacity_is_sufficient() && search(depth + 1)) {
            return 1;
        }
        for (int clue = 0; clue < CLUES; ++clue) {
            matches[clue] -= (unsigned char)(digit == guesses[clue][best_position] - '0');
        }
    }
    filled[best_position] = 0;
    return 0;
}

int main(void) {
    if (!search(0)) {
        return 1;
    }
    puts(answer);
    return 0;
}
