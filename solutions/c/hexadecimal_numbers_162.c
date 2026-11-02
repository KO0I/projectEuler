/* Project Euler 162: Hexadecimal Numbers
 *
 * For each length 3..16, inclusion-exclusion enforces the presence of digits
 * 0, 1, and A.  A mask says which required symbols are absent, leaving an
 * alphabet of 16-popcount(mask) symbols.  If zero is excluded, every remaining
 * symbol may lead; otherwise the leading position has one fewer choice.
 *
 * Alternating signs by subset size gives the valid count for that length.
 * Counts fit uint64_t, while the inclusion-exclusion accumulator is signed.
 * The requested answer is formatted directly in uppercase hexadecimal. */
#include "euler_extra.h"
static uint64_t power162(unsigned base,unsigned exponent){uint64_t r=1;while(exponent--)r*=base;return r;}
int main(void){uint64_t answer=0;for(unsigned length=3;length<=16;++length){int64_t count=0;
    for(unsigned mask=0;mask<8;++mask){unsigned removed=__builtin_popcount(mask),alphabet=16-removed;
        uint64_t ways=(mask&1)?power162(alphabet,length):(uint64_t)(alphabet-1)*power162(alphabet,length-1);
        count+=(removed&1)?-(int64_t)ways:(int64_t)ways;}answer+=count;}
    printf("%" PRIX64 "\n",answer);return 0;}
