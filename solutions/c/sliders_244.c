#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Project Euler 244: Sliders
 *
 * A board is completely determined by a 16-bit red-tile mask and the blank's
 * four-bit position; blue occupies everything else.  This compact encoding
 * gives a direct table of only 2^20 possible keys, avoiding a hash table.
 *
 * Breadth-first search discovers boards in shortest-distance order.  Along
 * with distance, each board stores the number N of shortest prefixes and the
 * sum C of their rolling checksums.  Extending every prefix by letter a gives
 *
 *     N' += N,
 *     C' += 243*C + ASCII(a)*N  (mod 100000007).
 *
 * Therefore different shortest prefixes can merge into one state without
 * losing their path-dependent checksums.  The move letters describe the tile,
 * so a blank moving right corresponds to a tile moving left ('L'), etc.
 */

enum { STATE_COUNT = 1 << 20 };
static const uint64_t CHECK_MOD = UINT64_C(100000007);

static uint32_t make_state(uint16_t red, int blank) {
    return (uint32_t)red | ((uint32_t)blank << 16);
}

int main(void) {
    /* Red positions read row-major from the two diagrams. */
    const uint16_t start_red = (uint16_t)((1U << 1) | (1U << 4) | (1U << 5)
        | (1U << 8) | (1U << 9) | (1U << 12) | (1U << 13));
    const uint16_t target_red = (uint16_t)((1U << 2) | (1U << 5) | (1U << 7)
        | (1U << 8) | (1U << 10) | (1U << 13) | (1U << 15));
    const uint32_t start = make_state(start_red, 0);
    const uint32_t target = make_state(target_red, 0);

    unsigned char *distance = malloc(STATE_COUNT);
    uint64_t *path_count = calloc(STATE_COUNT, sizeof(*path_count));
    uint64_t *checksum_sum = calloc(STATE_COUNT, sizeof(*checksum_sum));
    uint32_t *queue = malloc(STATE_COUNT * sizeof(*queue));
    if (!distance || !path_count || !checksum_sum || !queue)
        return 1;
    memset(distance, 0xff, STATE_COUNT); /* 255 denotes undiscovered */

    size_t head = 0, tail = 0;
    queue[tail++] = start;
    distance[start] = 0;
    path_count[start] = 1;

    while (head < tail) {
        const uint32_t state = queue[head++];
        if (state == target)
            break; /* all distance-(D-1) predecessors were already processed */
        const uint16_t red = (uint16_t)(state & 0xffffU);
        const int blank = (int)(state >> 16);
        const int x = blank & 3;
        const int y = blank >> 2;

        /* Entries are blank displacement and the opposite tile direction. */
        static const int dx[4] = {1, -1, 0, 0};
        static const int dy[4] = {0, 0, 1, -1};
        static const unsigned char letter[4] = {'L', 'R', 'U', 'D'};
        for (int move = 0; move < 4; ++move) {
            const int nx = x + dx[move], ny = y + dy[move];
            if (nx < 0 || nx >= 4 || ny < 0 || ny >= 4)
                continue;
            const int neighbor = ny * 4 + nx;
            uint16_t next_red = red;
            if (red & (uint16_t)(1U << neighbor)) {
                next_red &= (uint16_t)~(1U << neighbor);
                next_red |= (uint16_t)(1U << blank);
            }
            const uint32_t next = make_state(next_red, neighbor);
            const unsigned char next_distance =
                (unsigned char)(distance[state] + 1);
            if (distance[next] == 255) {
                distance[next] = next_distance;
                queue[tail++] = next;
            }
            if (distance[next] != next_distance)
                continue; /* edge does not belong to a shortest prefix */

            path_count[next] += path_count[state];
            checksum_sum[next] =
                (checksum_sum[next] + 243 * checksum_sum[state]
                 + (uint64_t)letter[move] * path_count[state]) % CHECK_MOD;
        }
    }

    printf("%llu\n", (unsigned long long)checksum_sum[target]);
    free(queue);
    free(checksum_sum);
    free(path_count);
    free(distance);
    return 0;
}
