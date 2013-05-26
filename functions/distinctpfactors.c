#include<stdio.h>
#include<math.h>


//  Returns the NUMBER of distinct prime factors
//  'History' of past primes is done using an array
  int distinctpfactors(int n){
  int s=10,j,i=0,distinct=1;
  int f[10];
  // initialize f with zeros!
    for(j=0;j<s;j++){
      f[j]=0;
    }
  int d = 2;
  int distinctfacts = 0;
  if(n < 2) return 1;
  /* while the factor being tested
  is lower than the number to factorize */
    while(d < n) {
    /* if valid prime factor */
      if(n % d == 0) {
        //  even though the prime factor is valid
        //  is it distinct?
        for(j=0;j<=distinctfacts;j++){
          if(f[j]==d){//  not distinct
            distinct = 0;
            break;
          }else{      //  distinct
            distinct = 1;
          }
        }
        if(distinct == 1){
          f[i]=d;
          n/=d;
          i++;
          distinctfacts++; 
        }else{
          n/=d;
        }
      } else {
  /* else: invalid prime factor */
  if(d == 2) d = 3;
  else d += 2;
    }
  }
  //  add the last prime factor
  //  but first make sure it's distinct
  distinct = 1; //distinct until proven otherwise
  for(i=0;i<=distinctfacts;i++){
    if(f[i]==n) {
      distinct = 0;
      break;
    }
  }
  if(distinct == 1){
    distinctfacts++;
    f[distinctfacts]=n;
  }
  //for(i=0;i<facts; i++){
  //  printf("%d * ",f[i]);
  //}
  return distinctfacts;
}

