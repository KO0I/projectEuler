#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Project Euler 248: Euler's Totient Function Equals 13!
 *
 * If p^e divides n, its independent contribution to phi(n) is
 * (p-1)p^(e-1).  Hence p-1 must divide T=13!, so every possible prime divisor
 * is p=d+1 for one of T's mere 1584 divisors.  We generate and primality-test
 * that finite candidate list, then recursively choose increasing primes and
 * every exponent whose totient contribution still divides the remaining T.
 *
 * Increasing candidate indices give each prime-power factorization one unique
 * path.  The recursion emits every inverse-totient value, which is sorted to
 * select the requested 150000th entry rather than embedding that answer.
 */

typedef uint64_t U64;
static const U64 TARGET=UINT64_C(6227020800);
static U64 divisors[1584];static int divisor_count;
static U64 candidates[1584];static int candidate_count;
static U64 *solutions;static size_t solution_count,solution_capacity;
static int small_primes[8000],small_count;

static int compare_u64(const void *a,const void *b){U64 x=*(const U64*)a,y=*(const U64*)b;return x<y?-1:x>y;}
static int is_prime(U64 n){if(n<2)return 0;for(int i=0;i<small_count&&(U64)small_primes[i]*small_primes[i]<=n;++i)if(n%(U64)small_primes[i]==0)return n==(U64)small_primes[i];return 1;}

static void make_divisors(int at,U64 value){
    static const U64 prime[]={2,3,5,7,11,13};static const int max_exp[]={10,5,2,1,1,1};
    if(at==6){divisors[divisor_count++]=value;return;}U64 power=1;for(int e=0;e<=max_exp[at];++e){make_divisors(at+1,value*power);power*=prime[at];}
}
static void emit(U64 n){if(solution_count==solution_capacity){solution_capacity=solution_capacity?2*solution_capacity:262144;U64 *grown=realloc(solutions,solution_capacity*sizeof(*solutions));if(!grown)exit(1);solutions=grown;}solutions[solution_count++]=n;}

static void generate(int start,U64 remaining,U64 n){
    if(remaining==1){emit(n);return;}
    for(int i=start;i<candidate_count;++i){U64 p=candidates[i],contribution=p-1,power=p;if(remaining%contribution)continue;
        while(remaining%contribution==0){if(n<=UINT64_MAX/power)generate(i+1,remaining/contribution,n*power);if(contribution>remaining/p)break;contribution*=p;if(power>UINT64_MAX/p)break;power*=p;}
    }
}

int main(void){
    unsigned char composite[80001]={0};for(int p=2;p<=80000;++p)if(!composite[p]){small_primes[small_count++]=p;if((int64_t)p*p<=80000)for(int m=p*p;m<=80000;m+=p)composite[m]=1;}
    make_divisors(0,1);
    for(int i=0;i<divisor_count;++i)if(is_prime(divisors[i]+1))candidates[candidate_count++]=divisors[i]+1;
    qsort(candidates,(size_t)candidate_count,sizeof(*candidates),compare_u64);
    generate(0,TARGET,1);
    qsort(solutions,solution_count,sizeof(*solutions),compare_u64);
    if(solution_count<150000)return 1;
    printf("%llu\n",(unsigned long long)solutions[149999]);free(solutions);return 0;
}
