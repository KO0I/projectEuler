#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 249: Prime Subset Sums
 *
 * dp[s] counts subsets of the primes below 5000 whose sum is s.  Processing
 * each prime once and updating sums downward implements the usual 0/1 subset
 * recurrence without accidentally reusing a prime.  The largest possible sum
 * is only about 1.55 million, so a dense table is practical.
 *
 * Only the rightmost 16 digits are requested; reducing every addition modulo
 * 10^16 is valid because all later operations are additions.  A second sieve
 * marks which attainable sums are prime, and those dp entries are accumulated.
 */

static const uint64_t MODULUS=UINT64_C(10000000000000000);

int main(void){
    enum{PRIME_LIMIT=5000};unsigned char composite[PRIME_LIMIT]={0};int primes[700],count=0,total_sum=0;
    for(int p=2;p<PRIME_LIMIT;++p)if(!composite[p]){primes[count++]=p;total_sum+=p;if(p*p<PRIME_LIMIT)for(int m=p*p;m<PRIME_LIMIT;m+=p)composite[m]=1;}
    uint64_t *dp=calloc((size_t)total_sum+1,sizeof(*dp));unsigned char *sum_composite=calloc((size_t)total_sum+1,1);if(!dp||!sum_composite)return 1;
    dp[0]=1;int current=0;
    for(int i=0;i<count;++i){int p=primes[i];for(int s=current;s>=0;--s)if(dp[s])dp[s+p]=(dp[s+p]+dp[s])%MODULUS;current+=p;}
    sum_composite[0]=sum_composite[1]=1;for(int p=2;p*p<=total_sum;++p)if(!sum_composite[p])for(int m=p*p;m<=total_sum;m+=p)sum_composite[m]=1;
    uint64_t answer=0;for(int s=2;s<=total_sum;++s)if(!sum_composite[s])answer=(answer+dp[s])%MODULUS;
    printf("%016llu\n",(unsigned long long)answer);free(sum_composite);free(dp);return 0;
}
