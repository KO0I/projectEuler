/* Project Euler 191: Prize Strings
 *
 * The DP state records whether an absence has appeared and the current run of
 * consecutive lates (0,1,2).  Appending O resets the late run, A is allowed
 * only when `absent==0`, and L is allowed only while the run is shorter than
 * three.  Those two fields are the complete history relevant to future choices.
 *
 * Two rolling 2x3 tables advance exactly thirty days.  Summing all six terminal
 * states counts every valid attendance string once; uint64_t comfortably holds
 * the result. */
#include "euler_extra.h"
int main(void) {
    uint64_t dp[2][3] = {{1,0,0},{0,0,0}}, next[2][3];
    for (unsigned day = 0; day < 30; ++day) {
        memset(next, 0, sizeof(next));
        for (unsigned absent = 0; absent < 2; ++absent) for (unsigned late = 0; late < 3; ++late) {
            next[absent][0] += dp[absent][late];
            if (!absent) next[1][0] += dp[absent][late];
            if (late < 2) next[absent][late + 1] += dp[absent][late];
        }
        memcpy(dp, next, sizeof(dp));
    }
    uint64_t answer = 0;
    for (unsigned a = 0; a < 2; ++a) for (unsigned l = 0; l < 3; ++l) answer += dp[a][l];
    printf("%" PRIu64 "\n", answer);
    return 0;
}
