#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 245: Coresilience
 *
 * The unit-fraction condition is n-1=k(n-phi(n)).  A repeated prime factor
 * would divide n-phi(n) but not n-1, so every solution is squarefree.
 *
 * For n=pq, rearrangement gives (p-k)(q-k)=k^2-k+1.  We sieve the polynomial
 * values for every k<=sqrt(L), enumerate their divisor pairs, and recover p
 * and q.  A prime r divides the polynomial only in the roots of
 * x^2-x+1 mod r; Tonelli-Shanks obtains those roots, so factoring all 447213
 * values is a sieve rather than hundreds of thousands of trial divisions.
 *
 * For three or more primes, after choosing m=product(q_i) and
 * A=product(q_i-1), the final prime is forced:
 *
 *   p=(kA+1)/(kA-(k-1)m).
 *
 * A DFS over increasing primes tests that closure and stops as soon as the
 * denominator is nonpositive or there is no room for two further primes.
 */

typedef uint64_t U64;
typedef unsigned __int128 U128;
enum { KMAX = 447213, MAX_FACTORS = 8 };
static const U64 LIMIT = UINT64_C(200000000000);
static int *primes, prime_count;
static U64 answer;
static U64 *remaining;
static uint32_t *factor;
static unsigned char *exponent, *factor_count;

static U64 mulmod(U64 a,U64 b,U64 m){return (U64)((U128)a*b%m);}
static U64 powmod(U64 a,U64 e,U64 m){U64 r=1;while(e){if(e&1)r=mulmod(r,a,m);a=mulmod(a,a,m);e>>=1;}return r;}
static int prime64(U64 n){
    if(n<2)return 0;
    static const U64 small[]={2,3,5,7,11,13,17,19,23,29,31,37};
    for(size_t i=0;i<sizeof(small)/sizeof(*small);++i){if(n==small[i])return 1;if(n%small[i]==0)return 0;}
    U64 d=n-1,s=0;while(!(d&1)){d>>=1;++s;} static const U64 bases[]={2,325,9375,28178,450775,9780504,1795265022};
    for(size_t i=0;i<sizeof(bases)/sizeof(*bases);++i){U64 a=bases[i]%n;if(!a)continue;U64 x=powmod(a,d,n);if(x==1||x==n-1)continue;int ok=0;for(U64 j=1;j<s;++j){x=mulmod(x,x,n);if(x==n-1){ok=1;break;}}if(!ok)return 0;}return 1;
}

/* Square root modulo an odd prime; calls occur only when a root exists. */
static U64 modsqrt(U64 n,U64 p){
    n%=p;if(n==0)return 0;if(p%4==3)return powmod(n,(p+1)/4,p);
    U64 q=p-1,s=0;while(!(q&1)){q>>=1;++s;}U64 z=2;while(powmod(z,(p-1)/2,p)!=p-1)++z;
    U64 c=powmod(z,q,p),x=powmod(n,(q+1)/2,p),t=powmod(n,q,p),m=s;
    while(t!=1){U64 i=1,v=mulmod(t,t,p);while(v!=1){v=mulmod(v,v,p);++i;}U64 b=powmod(c,UINT64_C(1)<<(m-i-1),p);x=mulmod(x,b,p);c=mulmod(b,b,p);t=mulmod(t,c,p);m=i;}return x;
}

static void strip_progression(int p,int root){
    int k=root;while(k<2)k+=p;
    for(;k<=KMAX;k+=p){size_t base=(size_t)k*MAX_FACTORS;if(remaining[k]%p)continue;unsigned char e=0;do{remaining[k]/=p;++e;}while(remaining[k]%p==0);unsigned char c=factor_count[k];if(c<MAX_FACTORS){factor[base+c]=(uint32_t)p;exponent[base+c]=e;factor_count[k]=(unsigned char)(c+1);}}
}

static void divisors(int k,int at,U64 value,U64 polynomial){
    size_t base=(size_t)k*MAX_FACTORS;
    if(at==factor_count[k]){
        U64 other=polynomial/value;if(value>other)return;U64 p=(U64)k+value,q=(U64)k+other;
        if(p!=q&&prime64(p)&&prime64(q)&&p<=LIMIT/q)answer+=p*q;
        return;
    }
    U64 power=1;for(int e=0;e<=exponent[base+at];++e){divisors(k,at+1,value*power,polynomial);power*=factor[base+at];}
}

static int power_fits(U64 base,int exponent,U64 bound){
    U64 value=1;for(int i=0;i<exponent;++i){if(value>bound/base)return 0;value*=base;}return 1;
}

/* Given partial n=A and phi=B, solve algebraically for every possible unit
   denominator u and its forced final prime r.  Bounds on r translate into a
   very short interval of u values. */
static void try_last_prime(U64 a,U64 b,U64 last){
    U64 d=a-b,rmax=LIMIT/a;if(d==0||rmax<=last)return;
    U64 umin=(b*(last-1)-1)/(b+last*d)+1;
    if(umin<1)umin=1;
    U64 umax=(b*(rmax-1)-1)/(b+rmax*d);
    U64 cap=(b-1)/d;if(umax>cap)umax=cap;
    for(U64 u=umin;u<=umax;++u){U64 denominator=b-u*d;if(denominator==0)break;U64 numerator=b*(u+1)+1;if(numerator%denominator)continue;U64 r=numerator/denominator;if(r>last&&a<=LIMIT/r&&prime64(r))answer+=a*r;}
}

static void factor_dfs(int index,int remaining_count,U64 a,U64 b,U64 last){
    if(remaining_count==0){try_last_prime(a,b,last);return;}
    U64 bound=LIMIT/a;
    for(int i=index;i<prime_count;++i){U64 p=(U64)primes[i];if(!power_fits(p,remaining_count+1,bound))break;factor_dfs(i+1,remaining_count-1,a*p,b*(p-1),p);}
}

int main(void){
    unsigned char *composite=calloc(KMAX+1,1);primes=malloc((KMAX/2)*sizeof(*primes));
    remaining=malloc((KMAX+1)*sizeof(*remaining));factor=calloc((size_t)(KMAX+1)*MAX_FACTORS,sizeof(*factor));
    exponent=calloc((size_t)(KMAX+1)*MAX_FACTORS,1);factor_count=calloc(KMAX+1,1);
    if(!composite||!primes||!remaining||!factor||!exponent||!factor_count)return 1;
    for(int p=2;p<=KMAX;++p)if(!composite[p]){primes[prime_count++]=p;if((int64_t)p*p<=KMAX)for(int x=p*p;x<=KMAX;x+=p)composite[x]=1;}
    for(int k=2;k<=KMAX;++k)remaining[k]=(U64)k*k-k+1;
    strip_progression(3,2);
    for(int i=0;i<prime_count;++i){int p=primes[i];if(p==3||p%3!=1)continue;U64 root=
        modsqrt((U64)p-3,p); /* -3 mod p */
        U64 inv2=(p+1)/2;int r1=(int)mulmod(1+root,inv2,p);int r2=(int)mulmod(1+p-root,inv2,p);strip_progression(p,r1);if(r2!=r1)strip_progression(p,r2);
    }
    for(int k=2;k<=KMAX;++k){if(remaining[k]>1){size_t b=(size_t)k*MAX_FACTORS;unsigned char c=factor_count[k];factor[b+c]=(uint32_t)remaining[k];exponent[b+c]=1;factor_count[k]=(unsigned char)(c+1);}divisors(k,0,1,(U64)k*k-k+1);}
    int maximum_factors=0;U64 product=1;
    for(int i=1;i<prime_count;++i){if(product>LIMIT/(U64)primes[i])break;product*=(U64)primes[i];++maximum_factors;}
    for(int count=3;count<=maximum_factors;++count)
        factor_dfs(1,count-1,1,1,2); /* index 1 skips prime 2: valid n is odd */
    printf("%llu\n",(unsigned long long)answer);
    free(factor_count);free(exponent);free(factor);free(remaining);free(primes);free(composite);return 0;
}
