#include <stdio.h>

//here's the code that let's you import the numbers in the million_primes file
//to a giant array!
//
//Sadly, I haven't given enough fucks to try and figure out how to make it a
//function like isPrime. 

int main(){
//First, let's pull in some of the prime number list as an array.
FILE *primefile;

if((primefile = fopen("million_primes","r")) == NULL) {
  printf("cannot open million_primes for reading.\n");
  return 1;
}

int prime[1000000];
int n, i = 0;
while(fscanf(primefile, "%d", &n) > 0) {
  prime[i] = n;
  i++;
}
fclose(primefile);

//printf("%d\n",prime[0]);
return 0;
}
