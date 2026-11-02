#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 198: ambiguous numbers.
 *
 * Two equally good rational approximations are Farey neighbours a/b<c/d
 * (bc-ad=1), and the ambiguous number is their arithmetic midpoint.  That
 * midpoint is reduced and has denominator 2bd.  We therefore visit the
 * Stern-Brocot intervals, which enumerate every neighbouring pair exactly
 * once, and test the two integer conditions
 *
 *     bd <= 50,000,000,
 *     100(ad+bc) < 2bd.
 *
 * A naive traversal has a 50-million-node spine
 * (0/1,1/1), (0/1,1/2), ... .  Those midpoint values are simply 1/(2d), so
 * the qualifying part of the spine is counted in one subtraction.  Every
 * off-spine interval belongs to the right subtree rooted at
 * (1/(d+1),1/d).  Only d>=100 can have values below 1/100, and d(d+1) must
 * fit the denominator-product bound, leaving a small collection of roots.
 * An explicit stack then explores their mediant children without recursion.
 */

static const uint64_t PRODUCT_LIMIT = UINT64_C(50000000);

typedef struct {
    uint64_t a;
    uint64_t b;
    uint64_t c;
    uint64_t d;
} Interval;

static int midpoint_is_below_limit(const Interval interval) {
    return UINT64_C(100) * (interval.a * interval.d + interval.b * interval.c) <
           2 * interval.b * interval.d;
}

static int left_endpoint_is_below_limit(const Interval interval) {
    return UINT64_C(100) * interval.a < interval.b;
}

static int push(Interval **stack, size_t *size, size_t *capacity, Interval value) {
    if (*size == *capacity) {
        const size_t grown_capacity = *capacity == 0 ? 1024 : 2 * *capacity;
        Interval *grown = realloc(*stack, grown_capacity * sizeof(*grown));
        if (grown == NULL) {
            return 0;
        }
        *stack = grown;
        *capacity = grown_capacity;
    }
    (*stack)[(*size)++] = value;
    return 1;
}

int main(void) {
    /* On the zero-left spine, 1/(2d)<1/100 exactly when d>50. */
    uint64_t count = PRODUCT_LIMIT - 50;

    Interval *stack = NULL;
    size_t size = 0;
    size_t capacity = 0;

    /* The roots partition all Stern-Brocot intervals not on the zero spine. */
    for (uint64_t d = 100; d * (d + 1) <= PRODUCT_LIMIT; ++d) {
        if (!push(&stack, &size, &capacity, (Interval){1, d + 1, 1, d})) {
            free(stack);
            return 1;
        }
    }

    while (size != 0) {
        const Interval interval = stack[--size];
        if (interval.b * interval.d > PRODUCT_LIMIT ||
            !left_endpoint_is_below_limit(interval)) {
            continue;
        }
        if (midpoint_is_below_limit(interval)) {
            ++count;
        }

        const uint64_t mediant_numerator = interval.a + interval.c;
        const uint64_t mediant_denominator = interval.b + interval.d;
        const Interval left = {
            interval.a, interval.b, mediant_numerator, mediant_denominator
        };
        const Interval right = {
            mediant_numerator, mediant_denominator, interval.c, interval.d
        };

        /* Child denominator products are monotone.  Checking before pushing
           keeps the stack proportional to the live DFS frontier. */
        if (left.b * left.d <= PRODUCT_LIMIT &&
            !push(&stack, &size, &capacity, left)) {
            free(stack);
            return 1;
        }
        if (right.b * right.d <= PRODUCT_LIMIT &&
            left_endpoint_is_below_limit(right) &&
            !push(&stack, &size, &capacity, right)) {
            free(stack);
            return 1;
        }
    }

    printf("%llu\n", (unsigned long long)count);
    free(stack);
    return 0;
}
