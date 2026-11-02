/* https://projecteuler.net/problem=84
 * Deterministic Markov calculation for cyclic decks, not a random simulation.
 * State is (square, consecutive doubles, next CC card, next CH card), so cards
 * return to the bottom exactly as specified. Since the initial shuffle is not
 * supplied, use the explicit valid shuffled orders below. Fine probabilities
 * depend on that order; the requested three-square modal string is also
 * checked with a different ordering in tests/c_middle_examples.c. For six
 * sided dice these two orders even exchange GO and square 19 in third place;
 * the example percentages are not treated as universal for every shuffle.
 * Power iteration stops only when the full distribution's L1 change is small.
 */
#include "euler_extra.h"

enum { P84_STATES = 40 * 3 * 16 * 16, P84_MAX_OUTCOMES = 21 };
enum { P84_STAY = -1, P84_NEXT_R = -2, P84_NEXT_U = -3, P84_BACK = -4 };
typedef struct { unsigned sum, doubles, ways; } P84Outcome;

static unsigned p84_index(unsigned square, unsigned doubles, unsigned cc, unsigned ch) {
    return ((square * 3 + doubles) * 16 + cc) * 16 + ch;
}

static unsigned p84_transition(unsigned state, P84Outcome roll, const int cc_deck[16], const int ch_deck[16]) {
    unsigned ch = state % 16; state /= 16;
    unsigned cc = state % 16; state /= 16;
    unsigned doubles = state % 3, square = state / 3;
    if (roll.doubles && doubles == 2) return p84_index(10, 0, cc, ch);
    doubles = roll.doubles ? doubles + 1 : 0;
    square = (square + roll.sum) % 40;
    bool jailed = square == 30;
    if (square == 7 || square == 22 || square == 36) {
        int card = ch_deck[ch]; ch = (ch + 1) % 16;
        if (card >= 0) { square = (unsigned)card; jailed = card == 10; }
        else if (card == P84_NEXT_R) square = square == 7 ? 15 : (square == 22 ? 25 : 5);
        else if (card == P84_NEXT_U) square = square == 22 ? 28 : 12;
        else if (card == P84_BACK) square -= 3;
    }
    /* CH3's back-three card lands on CC3, which draws another card. */
    if (square == 2 || square == 17 || square == 33) {
        int card = cc_deck[cc]; cc = (cc + 1) % 16;
        if (card >= 0) { square = (unsigned)card; jailed = card == 10; }
    }
    if (jailed) { square = 10; doubles = 0; }
    return p84_index(square, doubles, cc, ch);
}

static unsigned p84_modal(unsigned sides, unsigned order, double marginal[40]) {
    /* Both are possible outcomes of the initial shuffle. No RNG is involved. */
    static const int chest[2][16] = {
        {P84_STAY, 0, P84_STAY, P84_STAY, P84_STAY, P84_STAY, 10, P84_STAY,
         P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY},
        {10, P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY,
         P84_STAY, 0, P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY, P84_STAY}
    };
    static const int chance[2][16] = {
        {P84_STAY, P84_NEXT_R, 24, P84_STAY, 0, P84_NEXT_U, P84_STAY, 11,
         P84_BACK, 10, P84_STAY, 5, P84_NEXT_R, P84_STAY, 39, P84_STAY},
        {39, P84_STAY, P84_NEXT_U, P84_BACK, 10, P84_STAY, P84_STAY, P84_NEXT_R,
         11, P84_STAY, 0, 24, P84_STAY, P84_NEXT_R, P84_STAY, 5}
    };
    if (sides < 2 || sides > 6 || order > 1) euler_fail("Unsupported Monopoly parameters");
    P84Outcome outcomes[P84_MAX_OUTCOMES] = {{0, 0, 0}};
    size_t outcome_count = 0;
    for (unsigned a = 1; a <= sides; ++a) for (unsigned b = 1; b <= sides; ++b) {
        size_t i = 0;
        while (i < outcome_count && (outcomes[i].sum != a + b || outcomes[i].doubles != (a == b))) ++i;
        if (i == outcome_count) {
            if (outcome_count == P84_MAX_OUTCOMES) euler_fail("Too many dice outcomes");
            outcomes[outcome_count++] = (P84Outcome){a + b, a == b, 0};
        }
        ++outcomes[i].ways;
    }
    uint16_t *destinations = euler_malloc(P84_STATES * outcome_count * sizeof *destinations);
    for (unsigned state = 0; state < P84_STATES; ++state)
        for (size_t roll = 0; roll < outcome_count; ++roll)
            destinations[state * outcome_count + roll] = (uint16_t)p84_transition(state, outcomes[roll], chest[order], chance[order]);
    double *probability = euler_malloc(P84_STATES * sizeof *probability);
    double *next = euler_malloc(P84_STATES * sizeof *next);
    for (unsigned state = 0; state < P84_STATES; ++state) probability[state] = 1.0 / P84_STATES;
    bool converged = false;
    for (unsigned iteration = 0; iteration < 30000; ++iteration) {
        memset(next, 0, P84_STATES * sizeof *next);
        for (unsigned state = 0; state < P84_STATES; ++state) {
            double unit = probability[state] / (sides * sides);
            for (size_t roll = 0; roll < outcome_count; ++roll)
                next[destinations[state * outcome_count + roll]] += unit * outcomes[roll].ways;
        }
        double mass = 0, difference = 0;
        for (unsigned state = 0; state < P84_STATES; ++state) mass += next[state];
        for (unsigned state = 0; state < P84_STATES; ++state) {
            next[state] /= mass;
            difference += fabs(next[state] - probability[state]);
        }
        double *swap = probability; probability = next; next = swap;
        if (difference < 1e-13) { converged = true; break; }
    }
    if (!converged) euler_fail("Monopoly stationary distribution did not converge");
    for (unsigned square = 0; square < 40; ++square) marginal[square] = 0;
    for (unsigned state = 0; state < P84_STATES; ++state) marginal[state / (3 * 16 * 16)] += probability[state];
    unsigned ranking[40];
    for (unsigned i = 0; i < 40; ++i) ranking[i] = i;
    for (unsigned i = 0; i < 3; ++i) for (unsigned j = i + 1; j < 40; ++j)
        if (marginal[ranking[j]] > marginal[ranking[i]]) {
            unsigned swap = ranking[i]; ranking[i] = ranking[j]; ranking[j] = swap;
        }
    unsigned answer = 10000 * ranking[0] + 100 * ranking[1] + ranking[2];
    free(next); free(probability); free(destinations);
    return answer;
}

int main(void) {
    double marginal[40];
    printf("%06u\n", p84_modal(4, 0, marginal));
    return 0;
}
