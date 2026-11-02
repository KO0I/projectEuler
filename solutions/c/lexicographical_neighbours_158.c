/* Project Euler 158: https://projecteuler.net/problem=158 */
#include "euler_extra.h"
int main(void){uint64_t choose=1,best=0,power=1;for(unsigned n=1;n<=26;++n){
    choose=choose*(27-n)/n;power*=2;uint64_t value=choose*(power-n-1);if(value>best)best=value;}
    printf("%" PRIu64 "\n",best);return 0;}
