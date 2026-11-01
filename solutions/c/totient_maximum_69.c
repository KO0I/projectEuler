//Totient Maximum
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


//Passing the Totient function int i gives
unsigned long phi(unsigned long i){
    unsigned long res,j;
    if(i==1) return 1;
    res = i;
    //Starting with 2, check for divisibility
    if(i%2==0){
      res -= res/2;
      do i/=2;
      while (i%2==0);
    }
    //starting with 3 and skipping even numbers, check odds
    for (j=3; j*j<=i; j+=2){
      if(i%j==0){
        res -= res/j;
        do i/=j;
        while(i%j==0);
      }
    }
    if(i>1){
      res -= res/i;
    }
    return res;
}

int main(){
  double test=0,maxval=1,maxpos=1;
  unsigned long n;
  for(n=2;n<1000000;n++){
    //test is n/phi(n)
    test=(double) n/phi(n);
    //printf("n=%lu   Φ(%lu)=%lu   %lu/Φ(%lu)=%f\n",n,n,phi(n),n,n,test);
    if(test>maxval){
      maxval=(double)n/phi(n);
      maxpos=n;
      printf("%f>%f\n",test,maxval);
    }//else printf("%f<%f\n",test,maxval);
  }
  printf("\nMax Totient at n=%lu so Φ(n)=%lu\n\n",(unsigned long)maxpos,phi((unsigned long)maxpos));
  return 0;
}

