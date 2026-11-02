#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 215: crack-free walls.
 *
 * A row made from length-2 and length-3 bricks is represented by a bit mask:
 * bit x is set when a brick joint occurs x units from the left edge.  The two
 * outside edges are omitted.  Two consecutive rows are compatible exactly
 * when their masks have no common bit, which is precisely the absence of a
 * vertical crack crossing that boundary.
 *
 * Recursively generating the row tilings produces only a few thousand masks.
 * A height DP then sums ways ending in every mask, using the compatibility
 * test for each adjacent pair.  Counts fit in uint64_t for width 32, height 10.
 */

enum { WIDTH = 32, HEIGHT = 10, MAX_ROWS = 10000 };

static uint32_t masks[MAX_ROWS];
static int mask_count;

static void generate_rows(int position, uint32_t mask) {
    if (position == WIDTH) {
        masks[mask_count++] = mask;
        return;
    }
    for (int brick = 2; brick <= 3; ++brick) {
        const int end = position + brick;
        if (end <= WIDTH) {
            const uint32_t next_mask = end == WIDTH ? mask : mask | (UINT32_C(1) << end);
            generate_rows(end, next_mask);
        }
    }
}

int main(void) {
    generate_rows(0, 0);
    uint64_t ways[MAX_ROWS];
    uint64_t next[MAX_ROWS];
    for (int i = 0; i < mask_count; ++i) {
        ways[i] = 1;
    }
    for (int height = 1; height < HEIGHT; ++height) {
        for (int i = 0; i < mask_count; ++i) {
            next[i] = 0;
            for (int j = 0; j < mask_count; ++j) {
                if ((masks[i] & masks[j]) == 0) {
                    next[i] += ways[j];
                }
            }
        }
        for (int i = 0; i < mask_count; ++i) {
            ways[i] = next[i];
        }
    }
    uint64_t total = 0;
    for (int i = 0; i < mask_count; ++i) {
        total += ways[i];
    }
    printf("%llu\n", (unsigned long long)total);
    return 0;
}
