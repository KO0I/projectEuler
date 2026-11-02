#include <stdio.h>

//  Problem 30

//  integer powier function
int powi(int base, int exp){
  if (exp == 0) return 1;
  if (exp <  0) return 0;
  int i,res = 1;
    for(i=0; i<exp; i++){
      res *= base;
    }
  return res;
}

int main(){
  //  end condition to be determined by trial number s
  int s = 1000000;
  int j,k,answer=0;
  int dig = 5; //testing fourth powiers first
  //  solutions will be placed into an array

  for(j=2; j<=s; j++){
    //  first, break up the number into integers.
    //  this program will account for up to the number of digits in s (here, 5)
    int a = (j/1)%10,
        b = (j/10)%10,
        c = (j/100)%10,
        d = (j/1000)%10,
        e = (j/10000)%10,
        f = (j/100000)%10,
        g = (j/1000000)%10;
    //printf("%d, %d, %d, %d, %d\n",e,d,c,b,a);
    //if any of the digits exceed the original number, immediately move on
//    if( (powi(a,dig)>j)||(powi(b,dig)>j)||(powi(c,dig)>j)||(powi(d,dig)>j)||(powi(e,dig)>j))
    
    //sum up the digits of the exponents and compare with j
    int sum;
    sum = ( powi(a,dig) + powi(b,dig) + powi(c,dig) + powi(d,dig) + powi(e,dig) + powi(f,dig) + powi(g,dig) );
    if(sum == j) {
      printf("%d is a result\n", sum);
      answer += sum;
    }
  }
  printf("%d\n",answer);
  return 0;
}
