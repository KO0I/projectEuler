#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

//Circular primes:
//if all "rotations" of a prime are themselves prime, then it is a circular
//prime.
//
//The number, 197, is called a circular prime because all rotations of the
//digits: 197, 971, and 719, are themselves prime.
//
//There are thirteen such primes below 100: 2, 3, 5, 7, 11, 13, 17, 31, 37, 71,
//73, 79, and 97.

//the algorithm: each power of 10 will be independently analyzed
//Each number will be "rotated" and checked against the prime list
//if the rotated prime matches a known prime in the list, sum++

//test for all primes below 10:         (1 digit)
//  line: [0-4]


//test for all primes below 100:        (2 digits)
//  line: [5-25]

//test for all primes below 1000:       (3 digits)
//  line: [26-168]
int a(){
}

//test for all primes below 10000:      (4 digits)
//  line: [169-1229]
int b(){
}

//test for all primes below 100000:     (5 digits)
//  line: [1230-9592]
int c(){
}

//test for all primes below 1000000:    (6 digits)
//  line: [9592-78498]
int d(){
}

int main(){ 
  //all 4 single-digit primes are also circular, and 11 is a corner case
  int i,j,LLEN=2,sum=9;
  char line[LLEN]; 
  char out[2]; 
  const char filename[] = "circprimes";
  FILE *fp = fopen(filename, "r");

  if(fp==NULL){
    printf("failed to read the file.\n");
    return -1;
  }
  fclose(fp);
  return 0;
}
