/* Project Euler 161: Triominoes
 *
 * A profile DP fills the first empty cell of the current row.  Bit masks a,b,c
 * record occupancy in the current and next two rows—the deepest look-ahead any
 * straight or L triomino needs.  Anchoring every placement at the first empty
 * cell ensures a tiling has exactly one construction order.
 *
 * The six placements below are the two straight orientations and four L
 * orientations.  A full current row shifts the masks upward.  `(rows,a,b,c)`
 * is packed into a nonzero 64-bit key and memoized in an open-addressed table;
 * the final count fits uint64_t. */
#include "euler_extra.h"

enum { WIDTH161 = 9, CACHE161 = 1 << 20 };
static uint64_t keys161[CACHE161];
static uint64_t values161[CACHE161];

static size_t slot161(uint64_t key) {
    size_t slot = (size_t)((key * 11400714819323198485ULL) >> 44);
    while (keys161[slot] && keys161[slot] != key) slot = (slot + 1) & (CACHE161 - 1);
    return slot;
}

static uint64_t search161(unsigned rows, unsigned a, unsigned b, unsigned c) {
    if (rows == 0) return 1;
    const unsigned full = (1u << WIDTH161) - 1;
    if (a == full) return search161(rows - 1, b, c, 0);
    unsigned column = 0;
    while (a & (1u << column)) ++column;
    uint64_t state = rows;
    state = (state << WIDTH161) | a;
    state = (state << WIDTH161) | b;
    state = (state << WIDTH161) | c;
    uint64_t key = state + 1;
    size_t slot = slot161(key);
    if (keys161[slot]) return values161[slot];

    uint64_t result = 0;
    unsigned x = 1u << column;
    if (rows >= 2 && column + 1 < WIDTH161) {
        unsigned y = 1u << (column + 1);
        if (!(a & (x | y)) && !(b & x)) result += search161(rows, a | x | y, b | x, c);
        if (!(a & (x | y)) && !(b & y)) result += search161(rows, a | x | y, b | y, c);
        if (!(a & x) && !(b & (x | y))) result += search161(rows, a | x, b | x | y, c);
    }
    if (rows >= 2 && column > 0) {
        unsigned left = 1u << (column - 1);
        if (!(a & x) && !(b & (left | x))) result += search161(rows, a | x, b | left | x, c);
    }
    if (rows >= 3 && !(a & x) && !(b & x) && !(c & x))
        result += search161(rows, a | x, b | x, c | x);
    if (column + 2 < WIDTH161) {
        unsigned horizontal = x | (1u << (column + 1)) | (1u << (column + 2));
        if (!(a & horizontal)) result += search161(rows, a | horizontal, b, c);
    }
    keys161[slot] = key;
    values161[slot] = result;
    return result;
}

int main(void) {
    printf("%" PRIu64 "\n", search161(12, 0, 0, 0));
    return 0;
}
