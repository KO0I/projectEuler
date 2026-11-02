/* Project Euler 159: https://projecteuler.net/problem=159 */
#include "euler_extra.h"
int main(void){enum{LIMIT=1000000};unsigned char *m=malloc(LIMIT);if(!m)return EXIT_FAILURE;
    m[0]=0;for(unsigned n=1;n<LIMIT;++n)m[n]=1+(n-1)%9;
    for(unsigned a=2;a<LIMIT;++a)for(unsigned b=2;(uint64_t)a*b<LIMIT;++b){unsigned n=a*b,v=m[a]+m[b];if(v>m[n])m[n]=v;}
    uint64_t sum=0;for(unsigned n=2;n<LIMIT;++n)sum+=m[n];printf("%" PRIu64 "\n",sum);free(m);return 0;}
