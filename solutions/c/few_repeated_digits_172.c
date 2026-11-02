/* Project Euler 172: Few Repeated Digits
 *
 * Encode the usage count of each decimal digit in two bits of a 20-bit state.
 * At position `pos`, try every digit whose count is below three and increment
 * its two-bit field.  Zero is rejected only at the first position, exactly
 * enforcing an 18-digit number rather than a padded shorter number.
 *
 * Memoizing (position,state) merges every prefix with the same remaining digit
 * capacities.  A terminal position contributes one completed number; the full
 * count fits uint64_t. */
#include "euler_extra.h"
static uint64_t memo172[19][4*4*4*4*4*4*4*4*4*4];static bool seen172[19][1048576];
static uint64_t ways172(unsigned pos,unsigned state){if(pos==18)return 1;if(seen172[pos][state])return memo172[pos][state];seen172[pos][state]=true;uint64_t total=0;
    for(unsigned d=0;d<10;++d){if(pos==0&&d==0)continue;unsigned shift=2*d,c=(state>>shift)&3;if(c<3)total+=ways172(pos+1,state+(1u<<shift));}return memo172[pos][state]=total;}
int main(void){printf("%" PRIu64 "\n",ways172(0,0));return 0;}
