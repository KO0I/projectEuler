/* Project Euler 146: Investigating a Prime Pattern
 *
 * A valid n ends in zero.  Before performing expensive 64-bit primality
 * tests, sieve each candidate against small primes: if any required n^2+offset
 * is divisible by a filter prime, that n cannot work.  Survivors must make all
 * six specified offsets prime.
 *
 * The word "consecutive" also matters: offsets 19 and 21 between 13 and 27
 * must be composite.  Checking those two values prevents accepting a set that
 * has extra primes in the interval.  Since n<150 million, n^2 fits uint64_t. */
#include "euler_extra.h"

int main(void) {
    const unsigned offsets[] = {1,3,7,9,13,27};
    const unsigned filters[] = {3,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    uint64_t sum=0;
    for(uint64_t n=10;n<150000000;n+=10) {
        uint64_t square=n*n; bool possible=true;
        for(unsigned i=0;i<sizeof(filters)/sizeof(filters[0])&&possible;++i)
            for(unsigned j=0;j<sizeof(offsets)/sizeof(offsets[0]);++j)
                if((square+offsets[j])%filters[i]==0){possible=false;break;}
        if(!possible) continue;
        for(unsigned j=0;j<sizeof(offsets)/sizeof(offsets[0]);++j)
            if(!euler_is_prime(square+offsets[j])){possible=false;break;}
        if(possible && !euler_is_prime(square+19) && !euler_is_prime(square+21)) sum+=n;
    }
    printf("%" PRIu64 "\n",sum);
    return 0;
}
