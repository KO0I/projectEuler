#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 212: combined volume of cuboids.
 *
 * Sweep integer x slabs.  Cuboid endpoints are integers in [0,10398], and a
 * cuboid is active in every unit slab x0 <= x < x1.  Start/end linked lists
 * update a dense active array in O(1), so we never rescan all 50,000 cuboids.
 *
 * For each slab, compute the union area of the active yz rectangles with a
 * second sweep over y.  Its events add/remove a z interval in a segment tree;
 * the root always stores the currently covered z length.  The direct integer
 * z coordinate range avoids a separate compression map.  Events balance by
 * the end of every area sweep, returning the tree to its empty state for the
 * next x slab.
 */

enum { CUBOIDS = 50000, MAX_COORD = 10400, SEQUENCE_TERMS = 6 * CUBOIDS };

typedef struct {
    int x0, x1;
    int y0, y1;
    int z0, z1;
    int next_start;
    int next_end;
    int active_position;
} Cuboid;

typedef struct {
    int y;
    int z0;
    int z1;
    int delta;
} Event;

static int cover[4 * MAX_COORD];
static int covered_length[4 * MAX_COORD];

static void update_z(int node, int left, int right, int query_left, int query_right, int delta) {
    if (query_left >= right || query_right <= left) {
        return;
    }
    if (query_left <= left && right <= query_right) {
        cover[node] += delta;
    } else {
        const int middle = (left + right) / 2;
        update_z(2 * node, left, middle, query_left, query_right, delta);
        update_z(2 * node + 1, middle, right, query_left, query_right, delta);
    }
    if (cover[node] > 0) {
        covered_length[node] = right - left;
    } else if (right - left == 1) {
        covered_length[node] = 0;
    } else {
        covered_length[node] = covered_length[2 * node] + covered_length[2 * node + 1];
    }
}

static int event_compare(const void *left, const void *right) {
    const Event a = *(const Event *)left;
    const Event b = *(const Event *)right;
    return (a.y > b.y) - (a.y < b.y);
}

static uint64_t union_area(const Cuboid *cuboids, const int *active, int active_count,
                           Event *events) {
    int event_count = 0;
    for (int i = 0; i < active_count; ++i) {
        const Cuboid *box = &cuboids[active[i]];
        events[event_count++] = (Event){box->y0, box->z0, box->z1, 1};
        events[event_count++] = (Event){box->y1, box->z0, box->z1, -1};
    }
    qsort(events, (size_t)event_count, sizeof(*events), event_compare);

    uint64_t area = 0;
    int previous_y = event_count == 0 ? 0 : events[0].y;
    int index = 0;
    while (index < event_count) {
        const int y = events[index].y;
        area += (uint64_t)covered_length[1] * (uint64_t)(y - previous_y);
        while (index < event_count && events[index].y == y) {
            update_z(1, 0, MAX_COORD, events[index].z0, events[index].z1,
                     events[index].delta);
            ++index;
        }
        previous_y = y;
    }
    return area;
}

int main(void) {
    int *sequence = malloc((SEQUENCE_TERMS + 1) * sizeof(*sequence));
    Cuboid *cuboids = malloc(CUBOIDS * sizeof(*cuboids));
    Event *events = malloc(2 * CUBOIDS * sizeof(*events));
    int *active = malloc(CUBOIDS * sizeof(*active));
    if (sequence == NULL || cuboids == NULL || events == NULL || active == NULL) {
        free(active);
        free(events);
        free(cuboids);
        free(sequence);
        return 1;
    }

    for (int k = 1; k <= 55; ++k) {
        int64_t value = INT64_C(100003) - INT64_C(200003) * k +
                        INT64_C(300007) * k * k * k;
        value %= 1000000;
        sequence[k] = (int)(value < 0 ? value + 1000000 : value);
    }
    for (int k = 56; k <= SEQUENCE_TERMS; ++k) {
        sequence[k] = (sequence[k - 24] + sequence[k - 55]) % 1000000;
    }

    int start_head[MAX_COORD];
    int end_head[MAX_COORD];
    for (int x = 0; x < MAX_COORD; ++x) {
        start_head[x] = -1;
        end_head[x] = -1;
    }
    for (int i = 0; i < CUBOIDS; ++i) {
        const int k = 6 * i + 1;
        Cuboid *box = &cuboids[i];
        box->x0 = sequence[k] % 10000;
        box->y0 = sequence[k + 1] % 10000;
        box->z0 = sequence[k + 2] % 10000;
        box->x1 = box->x0 + 1 + sequence[k + 3] % 399;
        box->y1 = box->y0 + 1 + sequence[k + 4] % 399;
        box->z1 = box->z0 + 1 + sequence[k + 5] % 399;
        box->next_start = start_head[box->x0];
        start_head[box->x0] = i;
        box->next_end = end_head[box->x1];
        end_head[box->x1] = i;
        box->active_position = -1;
    }
    free(sequence);

    int active_count = 0;
    uint64_t volume = 0;
    for (int x = 0; x < MAX_COORD - 1; ++x) {
        for (int i = end_head[x]; i != -1; i = cuboids[i].next_end) {
            const int position = cuboids[i].active_position;
            const int moved = active[--active_count];
            active[position] = moved;
            cuboids[moved].active_position = position;
            cuboids[i].active_position = -1;
        }
        for (int i = start_head[x]; i != -1; i = cuboids[i].next_start) {
            cuboids[i].active_position = active_count;
            active[active_count++] = i;
        }
        volume += union_area(cuboids, active, active_count, events);
    }

    printf("%llu\n", (unsigned long long)volume);
    free(active);
    free(events);
    free(cuboids);
    return 0;
}
