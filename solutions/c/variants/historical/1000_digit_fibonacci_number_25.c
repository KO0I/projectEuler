#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

//Find the first 1000-digit Fibonacci term, and return that term

int main(void){
//For this, we need to use gmp:
mpz_t a,b,top, base, lentest;
unsigned long i=0,exp=999;
int u=-1;

mpz_init(a);
mpz_init(b);
mpz_init(top);
mpz_init(base);
mpz_init(lentest);

mpz_set_str(a,"1",10);
mpz_set_str(b,"1",10);
mpz_set_str(base,"1",10);
mpz_set_str(lentest,"1",10);
mpz_ui_pow_ui(top,10,exp);
//gmp_printf("%Zd\n",top);

int fibonacci(void){
//  hunt until a number with length n is found!
//  while(len<=n;){
    mpz_add(a,a,b);
    //gmp_printf("%Zd %Zd\n",a,b);
    mpz_swap(a,b);
    gmp_printf("%Zd ",a);
    i++;
    
}

//gmp_printf("%Zd %Zd\n",a,b);
while(u!=0){
  //if(a<=0){//complete!
  //  gmp_printf("Length: %Zd\n",lentest);
  //  printf("length:%d comp f'n out:%d \n",i,a);
  //  done=1;
  //}
  fibonacci();
  mpz_div(lentest,a,top);
  u = mpz_cmp_d(lentest,1);
  printf(" \n i: %i, u: %d",i,u);
  gmp_printf(" Length: %Zd \n",lentest);
  }
printf("%d\n",i);
//printf("%lu\n",i);
//printf("%lu\n",len);

return 0;
mpz_clear(a);
mpz_clear(b);
mpz_clear(top);
mpz_clear(base);
mpz_clear(lentest);
}
