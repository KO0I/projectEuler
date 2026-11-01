/* Project Euler 109: https://projecteuler.net/problem=109
 * The final dart is a double. Earlier darts are an unordered multiset
 * of labelled regions: equal scores in different regions remain distinct. */
#include "euler_extra.h"
int main(void) {
    unsigned score[62], count = 0, total = 0;
    for (unsigned multiplier = 1; multiplier <= 3; ++multiplier)
        for (unsigned value = 1; value <= 20; ++value)
            score[count++] = multiplier * value;
    score[count++] = 25; score[count++] = 50;
    for (unsigned finish = 1; finish <= 21; ++finish) {
        unsigned last = finish <= 20 ? 2 * finish : 50;
        ++total; /* A checkout consisting of the finishing dart alone. */
        for (unsigned i = 0; i < count; ++i) {
            if (score[i] + last < 100) ++total;
            for (unsigned j = i; j < count; ++j)
                if (score[i] + score[j] + last < 100) ++total;
        }
    }
    printf("%u\n", total);
    return 0;
}
