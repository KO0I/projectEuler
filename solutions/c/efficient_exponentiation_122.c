/* Project Euler 122: Efficient Exponentiation
 *
 * An addition chain starts at 1; appending a+b means multiplying already
 * available powers x^a and x^b.  We use iterative deepening, so the first
 * chain found for each target is provably shortest.  The search specializes
 * to star chains, where every new exponent adds the current largest exponent
 * to an earlier one; this suffices for the requested targets through 200.
 *
 * If doubling at every remaining level still cannot reach the target, the
 * branch is impossible and is pruned.  `seen` also merges different choices
 * that create the same next exponent at one node.  A 16-element chain is
 * ample because the discovered depths are far below that bound. */
#include "euler_extra.h"

static unsigned chain[16];

static bool reaches(unsigned target, unsigned depth, unsigned limit) {
    unsigned top = chain[depth];
    if (depth == limit) return top == target;
    if ((uint64_t)top << (limit - depth) < target) return false;
    bool seen[201] = {false};
    for (int i = (int)depth; i >= 0; --i) {
        unsigned next = top + chain[i];
        if (next <= top || next > target || seen[next]) continue;
        seen[next] = true;
        chain[depth + 1] = next;
        if (reaches(target, depth + 1, limit)) return true;
    }
    return false;
}

static unsigned minimum_multiplications(unsigned target) {
    chain[0] = 1;
    for (unsigned limit = 0;; ++limit)
        if (reaches(target, 0, limit)) return limit;
}

int main(void) {
    unsigned total = 0;
    for (unsigned exponent = 1; exponent <= 200; ++exponent)
        total += minimum_multiplications(exponent);
    printf("%u\n", total);
    return 0;
}
