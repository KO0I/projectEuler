/* Project Euler 186: https://projecteuler.net/problem=186

   Generate the exact telephone stream and maintain connected components with
   union-find.  Path compression and union by size make each successful call
   effectively constant time.  Misdials (caller==called) are discarded and do
   not increment the call counter, exactly as specified. */
#include "euler_extra.h"
enum { USERS186 = 1000000, PM186 = 524287 };
static unsigned parent186[USERS186], size186[USERS186], sequence186[55];
static unsigned find186(unsigned value) {
    if (parent186[value] != value) parent186[value] = find186(parent186[value]);
    return parent186[value];
}
static unsigned next186(uint64_t index) {
    unsigned value;
    if (index <= 55) {
        int64_t raw = 100003 - 200003 * (int64_t)index + 300007 * (int64_t)index * index * index;
        value = (raw % USERS186 + USERS186) % USERS186;
    } else value = (sequence186[(index - 25) % 55] + sequence186[(index - 56) % 55]) % USERS186;
    sequence186[(index - 1) % 55] = value;
    return value;
}
int main(void) {
    for (unsigned i = 0; i < USERS186; ++i) { parent186[i] = i; size186[i] = 1; }
    uint64_t index = 1, calls = 0;
    while (size186[find186(PM186)] < 990000) {
        unsigned caller = next186(index++), called = next186(index++);
        if (caller == called) continue;
        ++calls;
        unsigned a = find186(caller), b = find186(called);
        if (a == b) continue;
        if (size186[a] < size186[b]) { unsigned swap = a; a = b; b = swap; }
        parent186[b] = a; size186[a] += size186[b];
    }
    printf("%" PRIu64 "\n", calls);
    return 0;
}
