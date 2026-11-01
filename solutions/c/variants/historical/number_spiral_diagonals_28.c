#include<stdio.h>
#include<math.h>
/* Starting with the number 1 and moving to the right in a clockwise direction
 * a 5 by 5 spiral is formed as follows:
 *
 * 21 22 23 24 25
 * 20  7  8  9 10
 * 19  6  1  2 11
 * 18  5  4  3 12
 * 17 16 15 14 13
 *
 * It can be verified that the sum of the numbers on the diagonals is 101.
 *
 * What is the sum of the numbers on the diagonals in a 1001 by 1001 spiral
 * formed in the same way?
 */
long double sumA,sumB,sumC,sumD, i;


long double A(long double n){
  for(i=2;i<n;i++){
    sumA = powl((2*i+1), 2);
  }
  return sumA;
}

long double B(long double n){
  for(i=2;i<n;i++){
    sumB = (powl(i,2)*4 - 6*i + 3);
  }
  return sumB;
}

long double C(long double n){
  for(i=2;i<n;i++){
    sumC = (powl(i,2)*4 + 1);
  }
  return sumC;
}

long double D(long double n){
  for(i=2;i<n;i++){
    sumD = (powl(i,2)*4 -10*i + 7);
  } 
  return sumD;
}

int main(){
  /*  I found by looking at a larger Ulam spiral the sequence form of each
   *  diagonal "branch". This way, I did not have to generate the massive 
   *  1001 X 1001 number spiral. Sadly, this means that this code is somewhat
   *  anticlimatic.
   */
  long double sum = 0;

  /* Dimension of spiral */
  long double n = 5;

  sum = A(n)+B(n)+C(n)+D(n)+1;
  printf("Sum of Diagonals in an Ulam spiral of %ld by %ld is:\n  %ld\n",n,n,sum);

  return 0;
}
