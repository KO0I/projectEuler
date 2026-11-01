/* Project Euler 54: https://projecteuler.net/problem=54
 * Rank a hand by its category followed by all required tie breakers.
 * Base-15 packing makes integer comparison agree with poker ordering.
 * A,2,3,4,5 is treated as the five-high straight. */
#include "euler_extra.h"

static unsigned rank_value(char ch) {
    const char *ranks = "23456789TJQKA";
    const char *found = strchr(ranks, ch);
    if (!found) euler_fail("Invalid poker rank");
    return (unsigned)(found - ranks) + 2;
}

static uint64_t hand_value(const unsigned ranks[5], const char suits[5]) {
    unsigned count[15] = {0}, key[5] = {0}, category = 0;
    bool flush = true;
    for (unsigned i = 0; i < 5; ++i) {
        ++count[ranks[i]];
        if (suits[i] != suits[0]) flush = false;
    }
    unsigned descending[5], unique = 0, pairs[2], pair_count = 0, trip = 0, quad = 0;
    for (unsigned r = 14; r >= 2; --r) if (count[r]) {
        descending[unique++] = r;
        if (count[r] == 2) pairs[pair_count++] = r;
        if (count[r] == 3) trip = r;
        if (count[r] == 4) quad = r;
    }
    unsigned straight = 0;
    if (unique == 5) {
        if (descending[0] - descending[4] == 4) straight = descending[0];
        else if (count[14] && count[2] && count[3] && count[4] && count[5]) straight = 5;
    }
    if (flush && straight) { category = 8; key[0] = straight; }
    else if (quad) {
        category = 7; key[0] = quad;
        for (unsigned r = 2; r <= 14; ++r) if (count[r] == 1) key[1] = r;
    } else if (trip && pair_count) {
        category = 6; key[0] = trip; key[1] = pairs[0];
    } else if (flush) {
        category = 5;
        memcpy(key, descending, sizeof key);
    } else if (straight) { category = 4; key[0] = straight; }
    else if (trip) {
        category = 3; key[0] = trip;
        unsigned out = 1;
        for (unsigned r = 14; r >= 2; --r) if (count[r] == 1) key[out++] = r;
    } else if (pair_count == 2) {
        category = 2; key[0] = pairs[0]; key[1] = pairs[1];
        for (unsigned r = 2; r <= 14; ++r) if (count[r] == 1) key[2] = r;
    } else if (pair_count) {
        category = 1; key[0] = pairs[0];
        unsigned out = 1;
        for (unsigned r = 14; r >= 2; --r) if (count[r] == 1) key[out++] = r;
    } else memcpy(key, descending, sizeof key);
    uint64_t result = category;
    for (unsigned i = 0; i < 5; ++i) result = 15 * result + key[i];
    return result;
}

static bool first_player_wins(char *line) {
    static const char suit_names[] = "CDHS";
    unsigned ranks[10];
    char suits[10];
    uint64_t seen = 0;
    unsigned count = 0;
    for (char *token = strtok(line, " \t\r\n"); token; token = strtok(NULL, " \t\r\n")) {
        if (count == 10 || strlen(token) != 2) euler_fail("Invalid poker hand length");
        unsigned rank = rank_value(token[0]);
        const char *suit = strchr(suit_names, token[1]);
        if (!suit) euler_fail("Invalid poker suit");
        unsigned identity = 4 * (rank - 2) + (unsigned)(suit - suit_names);
        if (seen & (UINT64_C(1) << identity)) euler_fail("Repeated card in poker deal");
        seen |= UINT64_C(1) << identity;
        ranks[count] = rank;
        suits[count++] = token[1];
    }
    if (count != 10) euler_fail("Truncated poker deal");
    uint64_t first = hand_value(ranks, suits), second = hand_value(ranks + 5, suits + 5);
    if (first == second) euler_fail("Unexpected tied poker deal");
    return first > second;
}

int main(void) {
    FILE *file = euler_open("tests/data/p054_poker.txt");
    char line[128];
    unsigned deals = 0, wins = 0;
    while (fgets(line, sizeof line, file)) {
        if (!strchr(line, '\n') && !feof(file)) euler_fail("Poker input line is too long");
        wins += first_player_wins(line);
        ++deals;
    }
    if (ferror(file) || deals != 1000) euler_fail("Expected 1000 poker deals");
    fclose(file);
    printf("%u\n", wins);
    return 0;
}
