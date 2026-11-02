/* Project Euler 141: https://projecteuler.net/problem=141
 * Coprime b>c parameterize d=a*b^2, q=a*b*c, r=a*c^2, hence
 * n=d*q+r=a*c*(a*b^3+c). */
#include "euler_extra.h"

static bool square141(uint64_t n) {
    uint64_t r = (uint64_t)sqrt((double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r * r == n;
}
static int compare141(const void *l, const void *r) {
    uint64_t a=*(const uint64_t*)l,b=*(const uint64_t*)r;
    return a<b?-1:a!=b;
}
int main(void) {
    const uint64_t limit = 1000000000000ULL;
    uint64_t found[1024]; unsigned count=0;
    for (uint64_t b=2; b*b*b < limit; ++b) {
        uint64_t b3=b*b*b;
        for (uint64_t c=1;c<b;++c) {
            if (euler_gcd(b,c)!=1) continue;
            for (uint64_t a=1;;++a) {
                uint64_t n=a*c*(a*b3+c);
                if (n>=limit || n<a) break;
                if (square141(n)) found[count++]=n;
            }
        }
    }
    qsort(found,count,sizeof(*found),compare141);
    uint64_t sum=0;
    for(unsigned i=0;i<count;++i) if(!i||found[i]!=found[i-1]) sum+=found[i];
    printf("%" PRIu64 "\n",sum);
    return 0;
}
