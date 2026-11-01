/* Independent regression checks for the cubic-permutation grouping and
 * cyclic-deck Monopoly transition model. This deliberately uses a separate
 * rule interpreter rather than the production transition's control flow.
 * It checks every state and ordered pair of six-sided dice, including
 * card-pointer updates. No stationary-distribution iteration is repeated. */
#include <assert.h>
#define main p62_program
#include "../solutions/c/cubic_permutations_62.c"
#undef main
#define main p84_program
#include "../solutions/c/monopoly_odds_84.c"
#undef main

/* Independent step-by-step interpreter of the official board rules. */
static unsigned reference(unsigned state, unsigned a, unsigned b,
                          const int *cc_cards, const int *ch_cards) {
    unsigned original = state;
    unsigned ch = original % 16;
    unsigned cc = original / 16 % 16;
    unsigned streak = original / 256 % 3;
    unsigned pos = original / 768;
    streak = a == b ? streak + 1 : 0;
    if (streak == 3) { pos = 10; streak = 0; }
    else {
        pos = (pos + a + b) % 40;
        for (;;) {
            if (pos == 30) { pos = 10; streak = 0; break; }
            if (pos == 2 || pos == 17 || pos == 33) {
                int card = cc_cards[cc];
                cc = (cc + 1) % 16;
                if (card == P84_STAY) break;
                pos = (unsigned)card;
                if (pos == 10) { streak = 0; break; }
                continue;
            }
            if (pos == 7 || pos == 22 || pos == 36) {
                int card = ch_cards[ch];
                ch = (ch + 1) % 16;
                if (card == P84_STAY) break;
                if (card == P84_BACK) pos = (pos + 37) % 40;
                else if (card == P84_NEXT_R) {
                    do { pos = (pos + 1) % 40; } while (pos % 10 != 5);
                } else if (card == P84_NEXT_U) {
                    do { pos = (pos + 1) % 40; } while (pos != 12 && pos != 28);
                } else {
                    pos = (unsigned)card;
                    if (pos == 10) { streak = 0; break; }
                }
                continue;
            }
            break;
        }
    }
    return pos * 768 + streak * 256 + cc * 16 + ch;
}
int main(void) {
    int cc[16], ch[16];
    for (unsigned i = 0; i < 16; ++i) cc[i] = ch[i] = P84_STAY;
    cc[0] = 0; cc[1] = 10;
    const int moves[10] = {0,10,11,24,39,5,P84_NEXT_R,P84_NEXT_R,P84_NEXT_U,P84_BACK};
    for (unsigned i = 0; i < 10; ++i) ch[i] = moves[i];
    unsigned long checked = 0;
    for (unsigned state = 0; state < P84_STATES; ++state)
        for (unsigned a = 1; a <= 6; ++a)
            for (unsigned b = 1; b <= 6; ++b) {
                assert(reference(state,a,b,cc,ch) ==
                       p84_transition(state,(P84Outcome){a+b,a==b,1},cc,ch));
                ++checked;
            }
    printf("All %lu Monopoly transitions match independent rules\n",checked);
    P62Cube cubes[1000];
    size_t count = 0;
    for (uint64_t n = 1; n < 1000; ++n) {
        uint64_t cube = n*n*n;
        if (cube >= 10000000 && cube < 100000000)
            cubes[count++] = (P62Cube){p62_signature(cube),cube};
    }
    assert(p62_bucket(cubes,count,3) == UINT64_C(41063625));
    P62Cube six[6];
    for (unsigned i = 0; i < 6; ++i) six[i] = (P62Cube){123,100+i};
    assert(p62_bucket(six,6,5) == UINT64_MAX);
    puts("Cubic permutations exact-count and official three-cube example passed");
    return 0;
}
