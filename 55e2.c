//Lychrel numbers:
//
//For a number to be a candidate Lychrel number, it must surive 50
//iterations reverse-add-test and not become a palindrome.
//
//all numbers between 10 and 10000 will be tested
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

/* Initialize variables: */
  //number of lychrel numbers
  unsigned long lychrel=0;
/* begin testing at 1*/
  unsigned long test=1;
  unsigned long sum=0;
  unsigned long j=0;

/* Reverses an input string */
int strrev(char * in, char * out) {
  // check for well-formed input
  if (!in || !out) return -1;

  // find the end of the string in
  int i;
  for (i = 0; in[i] != '\0'; i++);

  // i should index the terminator of in
  assert (in[i] == '\0');

  // read in backwards, write out forwards
  for (i--; i >= 0; i--) {
    assert (in[i] != '\0');
    *out = in[i];
    out++;
  }
  // terminate out
  *out = '\0';

  // indicate success
  return 0;
}

/* tests if a number produces a palindrome for j iterations. */
int isLychrel(unsigned long t){
  /* t is a constant, temp, however, can be changed */
  unsigned long temp=t;
  char revsum[40];
  char tsum[40];
  int j;
  unsigned long b=1,f=1;
  char buf[40];
  char flip [40];
    /* 1. perform the reverse-and-add operation.
     * 2. check each produced number - if it's a palindrome, then it's not a
     * Lychrel number.
     * 3. if a number survives j rev&add operations, then it can be considered
     * a Lychrel number.
     */

    for(j=0; j<=50; j++){
      /* converts test case temp (lu) to a string called buf */
      sprintf(buf, "%lu", temp);
      /* flip the string called buf */
      strrev(buf,flip);
      /* convert the strings to integers */
      b = atoi(buf);
      f = atoi(flip);
      printf("\nj:%i, b:%lu, f:%lu ",j,b,f );
      /* test if the sum is a palindrome or not */
      sum=b+f;
      printf("sum:%lu, ",sum);
      /* set temp to equal this sum for the next iteration! */
      temp = sum;
      /* if j<0 and b=f, then the result is NOT a Lychrel number! */
      if((j<=1) && (b==f)){
          printf("%lu is not Lychrel after %i iterations! \n",temp,j+1);
        return 2;
        break;
      }
      /* If the Lychrel property undetermined, then prepare the next
       * iteration:
       */
      sprintf(buf, "%lu", sum);
      //printf("%s\n",buf);
    }
lychrel++;
return 1;
}


int main(){
  int i;
  /* run i number of test candidates */
  
  for(i=1; i<47; i++){
    isLychrel(i);
    }
  printf("Total Lychrel numbers: %lu\n", lychrel);
return 0;
}

