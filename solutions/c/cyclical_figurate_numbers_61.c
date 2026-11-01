/* https://projecteuler.net/problem=61
 * Enumerate four-digit polygonal numbers and backtrack through their two-digit
 * overlaps. Fixing the octagonal member first removes cyclic rotations. Each
 * polygonal type and each numeric value must occur exactly once. */
#include "euler_extra.h"

static int p61_values[6][150], p61_count[6], p61_path[6];

static int p61_search(int depth, unsigned used) {
    if (depth == 6) {
        if (p61_path[5] % 100 != p61_path[0] / 100) return 0;
        int sum = 0;
        for (int i = 0; i < 6; ++i) sum += p61_path[i];
        return sum;
    }
    int prefix = p61_path[depth - 1] % 100;
    for (int kind = 0; kind < 6; ++kind) if (!(used & (1u << kind))) {
        for (int i = 0; i < p61_count[kind]; ++i) {
            int value = p61_values[kind][i];
            if (value / 100 != prefix) continue;
            bool duplicate = false;
            for (int j = 0; j < depth; ++j) if (p61_path[j] == value) duplicate = true;
            if (duplicate) continue;
            p61_path[depth] = value;
            int answer = p61_search(depth + 1, used | (1u << kind));
            if (answer) return answer;
        }
    }
    return 0;
}

int main(void) {
    for (int kind = 0; kind < 6; ++kind) {
        int sides = kind + 3;
        for (int n = 1;; ++n) {
            int value = ((sides - 2) * n * n - (sides - 4) * n) / 2;
            if (value >= 10000) break;
            if (value >= 1000 && value % 100 >= 10) {
                if (p61_count[kind] == 150) euler_fail("Polygonal table capacity exceeded");
                p61_values[kind][p61_count[kind]++] = value;
            }
        }
    }
    for (int i = 0; i < p61_count[5]; ++i) {
        p61_path[0] = p61_values[5][i];
        int answer = p61_search(1, 1u << 5);
        if (answer) { printf("%d\n", answer); return 0; }
    }
    euler_fail("No cyclical set found");
}
