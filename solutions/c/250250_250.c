#include <stdint.h>
#include <stdio.h>

/*
 * Project Euler 250: 250250
 *
 * Only each i^i modulo 250 matters.  dp[r] counts subsets whose sum has
 * residue r; for the next value v, every old subset can be omitted or included,
 * producing residues r and (r+v) mod 250.  A fresh array per item prevents an
 * item from being selected repeatedly.
 *
 * Counts are reduced modulo 10^16 throughout because only the final 16 digits
 * are requested.  dp[0] includes the empty subset, which is removed at the end.
 */

static const uint64_t MODULUS=UINT64_C(10000000000000000);
static int power_mod_250(int base,int exponent){int result=1;base%=250;while(exponent){if(exponent&1)result=result*base%250;base=base*base%250;exponent>>=1;}return result;}

int main(void){
    uint64_t dp[250]={0},next[250];dp[0]=1;
    for(int i=1;i<=250250;++i){int value=power_mod_250(i,i);for(int r=0;r<250;++r)next[r]=dp[r];for(int r=0;r<250;++r){int target=(r+value)%250;next[target]=(next[target]+dp[r])%MODULUS;}for(int r=0;r<250;++r)dp[r]=next[r];}
    uint64_t answer=(dp[0]+MODULUS-1)%MODULUS;printf("%016llu\n",(unsigned long long)answer);return 0;
}
