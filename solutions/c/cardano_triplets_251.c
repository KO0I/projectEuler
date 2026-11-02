#include <math.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 251: Cardano Triplets
 *
 * Cubing the defining equation yields the integer parametrization
 *
 *   a=3pq-1, b=pr, c=sq^2,   8pq=sr^2+3,
 *
 * where r is odd and gcd(q,r)=1.  For fixed q,r the congruence
 * 8pq == 3 (mod r^2) has one residue p0 because 8q is invertible.  All
 * solutions are p=p0+t*r^2; s rises by 8q per step.  The sum constraint is
 * linear in t, so an entire arithmetic progression is counted at once.
 *
 * r=1 is handled separately with s=8pq-3.  The bound r^3<=8q(L+1) makes the
 * remaining double loop finite; a small conservative margin around cbrt is
 * harmless because the final exact inequality rejects excess candidates.
 */

static int64_t gcd64(int64_t a,int64_t b){while(b){int64_t t=a%b;a=b;b=t;}return a;}
static int64_t inverse_mod(int64_t a,int64_t m){
    int64_t old_r=a,r=m,old_s=1,s=0;while(r){int64_t q=old_r/r,t=old_r-q*r;old_r=r;r=t;t=old_s-q*s;old_s=s;s=t;}old_s%=m;if(old_s<0)old_s+=m;return old_s;
}

int main(void){
    const int64_t limit=110000000,L=limit+1;uint64_t total=0;
    int64_t qmax=(int64_t)sqrt((double)limit);while((qmax+1)*(qmax+1)<=limit)++qmax;while(qmax*qmax>limit)--qmax;
    for(int64_t q=1;q<=qmax;++q){
        int64_t q2=q*q;
        int64_t denominator=8*q*q2+3*q+1;
        total+=(uint64_t)((L+3*q2)/denominator); /* r=1, p=1..pmax */

        int64_t rmax=(int64_t)cbrt((double)(8*q*L))+3;if(!(rmax&1))++rmax;
        for(int64_t r=3;r<=rmax;r+=2){
            if(gcd64(q,r)!=1)continue;
            int64_t r2=r*r;
            int64_t p0=(3*inverse_mod((8*q)%r2,r2))%r2;if(p0==0)p0=r2;
            int64_t s0=(8*p0*q-3)/r2;
            int64_t base=p0*(3*q+r)+s0*q2;if(base>L)continue;
            int64_t step=r2*(3*q+r)+8*q*q2;
            total+=(uint64_t)((L-base)/step+1);
        }
    }
    printf("%llu\n",(unsigned long long)total);return 0;
}
