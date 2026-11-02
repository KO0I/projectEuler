/* Project Euler 171: Finding Numbers for Which the Sum of the Squares of the
 * Digits Is a Square
 *
 * For each attainable square-sum s, keep both the number of fixed-width
 * prefixes and the sum of their numeric values modulo 10^9.  Appending digit d
 * moves s to s+d^2, multiplies every old value by ten, and adds d once for
 * every old prefix.  Leading zeros are intentionally allowed: they provide a
 * unique 20-digit padded representation of every integer below 10^20.
 *
 * After twenty positions, sum the value totals at square indices.  The maximum
 * index is 20*9^2=1620, so two small rolling arrays cover the entire DP. */
#include "euler_extra.h"
int main(void){enum{MAX=20*81,MOD=1000000000};uint64_t count[MAX+1]={1},sum[MAX+1]={0},nc[MAX+1],ns[MAX+1];
    for(unsigned pos=0;pos<20;++pos){memset(nc,0,sizeof(nc));memset(ns,0,sizeof(ns));for(unsigned s=0;s<=pos*81;++s)for(unsigned d=0;d<10;++d){unsigned t=s+d*d;
        nc[t]=(nc[t]+count[s])%MOD;ns[t]=(ns[t]+sum[s]*10+count[s]*d)%MOD;}memcpy(count,nc,sizeof(count));memcpy(sum,ns,sizeof(sum));}
    uint64_t answer=0;for(unsigned r=1;r*r<=MAX;++r)answer=(answer+sum[r*r])%MOD;printf("%" PRIu64 "\n",answer);return 0;}
