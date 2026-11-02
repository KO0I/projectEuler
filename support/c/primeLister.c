#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

int main(void){

   long countDone = 1000000; //this is the number of primes the program will search for
   //
   //for problem 10, where all primes below 2 million are summed together, the
   //final prime is 1999993 and it is the 148933th prime
   //
   //for problem 35, we only consider primes less than 1 million - so only up
   //to the 78498th prime is relevant.
   long count = 0; 
   long prime[countDone]; 
   
   //set the first prime to 2, and the second prime to 3
   if(count == 0){
     prime[0]=2;
     prime[1]=3;
     count=2;
   }

   long candidate=5;                        //first candidate number is set
   assert(count>=2);
   for(count=2; count<=countDone; count++){ //this loop finds "countDone" number of primes
     double dcan = (double) candidate;      //(double) candidate;
     long sq = (long) ((sqrt(dcan))+0.5);   //sqrt of the candidate number
     long i;
     for(i=2; i<=sq; i++){                  //primality test for the candidate number (odds only)
       if(candidate % i == 0){              //if one of the numbers divides evenly into the candidate, 
                                            //it is a composite number
        candidate+=2;                       //prepare a new candidate and reset i.
        i=2;
      }
                                            //and then break the loop
    }
     //prime number is now added to prime[count]
     prime[count]=candidate;
     //prepare next candidate:
     candidate+=2;
  }


     //prints our prime list to a file named "million primes"
     const char filename[] = "million_primes";
     FILE *fp = fopen(filename,"w");

     if (fp==NULL){
       printf("failed to open file for write\n");
       return -1;
     }
     long j;
     for(j=0; j<=countDone; ++j){
       fprintf(fp, "%lu", prime[j]);
       fprintf(fp, "\n");
     }
     fclose(fp);
   return 0;
}


