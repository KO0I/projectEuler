#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 220: Heighway Dragon.
 *
 * A_0 and B_0 are empty; recursive expansion gives
 * A_n=A_(n-1) R B_(n-1) F R and
 * B_n=L F A_(n-1) L B_(n-1).  For every block and depth, precompute only its
 * displacement in a north-facing frame, net quarter-turn, and number of F
 * steps.  Summaries compose after rotating a child displacement by the
 * current heading.
 *
 * The requested prefix descends only into the one block containing its final
 * step.  Every preceding complete block is skipped in O(1) with its summary,
 * so D_50 requires a few dozen recursive decisions rather than constructing
 * its 2^50 forward moves.
 */

enum { ORDER = 50 };

typedef struct {
    int64_t x;
    int64_t y;
    int direction;
    uint64_t steps;
} State;

typedef State Summary;

static Summary a_summary[ORDER + 1];
static Summary b_summary[ORDER + 1];

static void rotate_vector(int64_t x, int64_t y, int direction, int64_t *rx, int64_t *ry) {
    switch (direction & 3) {
        case 0: *rx = x;  *ry = y;  break;
        case 1: *rx = y;  *ry = -x; break;
        case 2: *rx = -x; *ry = -y; break;
        default:*rx = -y; *ry = x;  break;
    }
}

static void apply_summary(State *state, Summary summary) {
    int64_t dx;
    int64_t dy;
    rotate_vector(summary.x, summary.y, state->direction, &dx, &dy);
    state->x += dx;
    state->y += dy;
    state->direction = (state->direction + summary.direction) & 3;
    state->steps += summary.steps;
}

static void turn(State *state, int quarter_turns) {
    state->direction = (state->direction + quarter_turns) & 3;
}

static void forward(State *state) {
    static const int dx[4] = {0, 1, 0, -1};
    static const int dy[4] = {1, 0, -1, 0};
    state->x += dx[state->direction];
    state->y += dy[state->direction];
    ++state->steps;
}

static Summary build_a(int depth) {
    State state = {0, 0, 0, 0};
    apply_summary(&state, a_summary[depth - 1]);
    turn(&state, 1);
    apply_summary(&state, b_summary[depth - 1]);
    forward(&state);
    turn(&state, 1);
    return state;
}

static Summary build_b(int depth) {
    State state = {0, 0, 0, 0};
    turn(&state, -1);
    forward(&state);
    apply_summary(&state, a_summary[depth - 1]);
    turn(&state, -1);
    apply_summary(&state, b_summary[depth - 1]);
    return state;
}

static void walk_block(int is_a, int depth, uint64_t *remaining, State *state) {
    if (*remaining == 0 || depth == 0) {
        return;
    }
    const Summary whole = is_a ? a_summary[depth] : b_summary[depth];
    if (whole.steps <= *remaining) {
        apply_summary(state, whole);
        *remaining -= whole.steps;
        return;
    }

    if (is_a) {
        walk_block(1, depth - 1, remaining, state);
        if (*remaining == 0) return;
        turn(state, 1);
        walk_block(0, depth - 1, remaining, state);
        if (*remaining == 0) return;
        forward(state);
        --*remaining;
        if (*remaining != 0) turn(state, 1);
    } else {
        turn(state, -1);
        forward(state);
        --*remaining;
        if (*remaining == 0) return;
        walk_block(1, depth - 1, remaining, state);
        if (*remaining == 0) return;
        turn(state, -1);
        walk_block(0, depth - 1, remaining, state);
    }
}

int main(void) {
    for (int depth = 1; depth <= ORDER; ++depth) {
        a_summary[depth] = build_a(depth);
        b_summary[depth] = build_b(depth);
    }

    State cursor = {0, 0, 0, 0};
    forward(&cursor); /* The initial F in D_n=F A_n. */
    uint64_t remaining = UINT64_C(1000000000000) - 1;
    walk_block(1, ORDER, &remaining, &cursor);
    printf("%lld,%lld\n", (long long)cursor.x, (long long)cursor.y);
    return 0;
}
