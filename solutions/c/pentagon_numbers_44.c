#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

//  Problem 44

//The following function creates the nth pentagon number.
long P(unsigned long n){
  long pentagon = 0;
  pentagon = (n*(3*n-1))/2;
  return pentagon;
}

int isPentagon(long x,int s, long pent[]){
  //returns 0 if true and returns 1 if false.
  int i;
  for(i=1; i<=s; i++){
    if(x == pent[i]){
      //  pentagon number found!
      return 0;
      break;
    }
  }
  // if the for loop doesn't find a match, return 1
  return 1;
}

int main(){
  //  check that the pentagon function isn't broken
  assert(P(5)==35);
  long i,j,k;
  int s=5000;
  long D=100;
  long pentagons[s+1];

  //  Generate s pentagon numbers:
  for(i=1; i<=s; i++){
    //  another check on the algorithm
    assert(P(i)>P(i-1));
    pentagons[i]=P(i);
    printf("P[%ld]=%ld\n",i,P(i));
  }
  long testsum,testdif, Pj, Pk;
  //  So BOTH the sum and difference of Pj,Pk must be pentagonal
  //  starting with the difference (easier to find).
  //  for general purposes, Pj > Pk
  
  for(k=1; k<=s; k++){
    for(j=2; j<=s; j++){
      Pj = pentagons[j];
      Pk = pentagons[k];
      testdif = Pj - Pk;
      testsum = Pj + Pk;
      if(!(isPentagon(testdif,s,pentagons))){
        //printf("For Pk=%ld, Pj=%d, dif is pentag'l",Pk,Pj);
        if(!(isPentagon(testdif,s,pentagons))){
          //printf(" and so is sum\n");
          //check to see if this is a new D (minimised value)
          if(labs(Pk-Pj)<=D) {
            D = labs(Pk-Pj);
            printf("new low score: %ld\n",D);
          }
        }else printf(".\n");
          
      }
    }//end of checking values for Pj
  }//end of checking values for Pk
  printf("lowest D=%ld\n",D);
  return 0;
}
