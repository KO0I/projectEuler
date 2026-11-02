#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 * Project Euler 240: Top Dice
 *
 * Twenty distinguishable 12-sided dice are rolled; only the ten largest
 * values must sum to 70.  Process face values from 12 down to 1.  When c dice
 * show the current face, min(c, remaining top slots) of them necessarily enter
 * the top ten.  Once ten top slots are occupied, lower faces affect only the
 * number of complete rolls, not the constrained sum.
 *
 * A state records (dice assigned, top slots assigned, top sum).  Choosing c of
 * the still-unassigned distinguishable dice contributes C(remaining,c).
 * Multiplying these choices over all faces is exactly the multinomial count
 * 20!/product(c_face!), so no roll is missed or duplicated.  We prune sums
 * above 70.  unsigned __int128 protects intermediate state counts even though
 * the requested final answer itself fits in 64 bits.
 */

enum { DICE = 20, TOP = 10, TARGET_SUM = 70 };
typedef unsigned __int128 Wide;

static Wide dp[DICE + 1][TOP + 1][TARGET_SUM + 1];
static Wide next_dp[DICE + 1][TOP + 1][TARGET_SUM + 1];
static uint64_t choose[DICE + 1][DICE + 1];

static void print_wide(Wide value) {
    char digits[64];
    int length = 0;
    do {
        digits[length++] = (char)('0' + value % 10);
        value /= 10;
    } while (value != 0);
    while (length != 0)
        putchar(digits[--length]);
    putchar('\n');
}

int main(void) {
    for (int n = 0; n <= DICE; ++n) {
        choose[n][0] = choose[n][n] = 1;
        for (int k = 1; k < n; ++k)
            choose[n][k] = choose[n - 1][k - 1] + choose[n - 1][k];
    }

    dp[0][0][0] = 1;
    for (int face = 12; face >= 1; --face) {
        memset(next_dp, 0, sizeof(next_dp));
        for (int used = 0; used <= DICE; ++used) {
            for (int top_used = 0; top_used <= TOP; ++top_used) {
                for (int sum = 0; sum <= TARGET_SUM; ++sum) {
                    const Wide ways = dp[used][top_used][sum];
                    if (ways == 0)
                        continue;
                    for (int count = 0; count <= DICE - used; ++count) {
                        int enters_top = count;
                        if (enters_top > TOP - top_used)
                            enters_top = TOP - top_used;
                        const int new_sum = sum + face * enters_top;
                        if (new_sum > TARGET_SUM)
                            continue;
                        next_dp[used + count][top_used + enters_top][new_sum]
                            += ways * choose[DICE - used][count];
                    }
                }
            }
        }
        memcpy(dp, next_dp, sizeof(dp));
    }

    print_wide(dp[DICE][TOP][TARGET_SUM]);
    return 0;
}
