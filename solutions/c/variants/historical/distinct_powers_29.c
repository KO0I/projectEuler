/* Distinct Powers:
 * Let a and b be integers on a range
 * if we let 2 <= a <= 5 and 2 <= b <= 5, then we get 15 distinct terms.
 * How many distinct terms are found in the sequence with the range from
 * 2 to 100
 */

/* NOTE: I cannot compute these values directly; the biggest ull I can use is
 * 18446744073709551615, but 100^100 is a tremendously larger number. We will
 * have to do mathematical analysis to take down this problem.
 *
 * as b varies, the output is exponential and will be larger than the
 * variation of a, which will be linear. Occasionally, values will intersect
 * and, in the case where a and b vary on a range of 2-5, there are 16
 * possible results, but only 15 are distinct. So on the range of 2-100, there
 * are, at best, 9801 distinct values. 
 *
 * Try 9800 - not successful!
 * 
 * Idea: There is a curve that intersects values of a^b that are the same
 */

#include <stdio.h>

int ipow(int base, int exp){
  //base^exp
  int result=1;
  while(exp){
    if(exp&1) result *= base;
    exp >>=1;
    base *= base;
  }
  return result;
}

unsigned int a; unsigned int b; unsigned int c;
int main(){
  for(a=2; a<7; a++){
    for(b=2; b<7; b++){
    c=ipow(a,b);
    printf("  %d   ",c);
    }
    printf("\n");
  }
  return 0;
}

