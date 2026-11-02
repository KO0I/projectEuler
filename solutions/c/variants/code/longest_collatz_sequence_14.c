#include <stdio.h>

//Longest Collatz sequence

unsigned long long collatz(unsigned long long n){
  if(n==0) return -1;
  unsigned long long len = 1;
  if(n==1) return len; //len is 1
  while(n != 1){
    if(n%2==0){ //even
      n/=2;
      len++;
    }else{       //if not even, then odd
      n*=3;
      n++;
      //if(n<1000000){ 
      len++;
      //Stupid me! I thought the problem read "... the terms are NOT allowed to
      //go above one million." READ THE PROBLEMS CAREFULLY.
      //}else{
      //  len = -1;
      //  return len;
      //}
    }
  }
  return len;
}

int main(){
  const unsigned long long CEIL = 1000000;
  unsigned long long i,max=0;
  unsigned long long maxi=2;
  for(i=2; i<CEIL; i++){
    if(collatz(i)>max){ 
      max=collatz(i);
      maxi=i;
    }
    if(collatz(i)>0) printf("%llu, %llu\n",i,collatz(i));
  }
  printf("\nlongest Collatz chain under %i is %i\nIt is produced by the starting number %i\n\n",CEIL,max,maxi);
  return 0;
}
