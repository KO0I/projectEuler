#include <stdio.h>
#include <assert.h>
#include <math.h>

int main(){
  int a,b,c;
  a=3;b=4;c=5;
  int sum = 1000;
  int sol = 0;
  int found = 0;
  while(found==0){
    for(a=1; a < (sum/3); a++){       //a can never be greater than 1/3 of the sum
      for (b=a; b < (sum/2); b++){    //b can never be greater than 1/2 of the sum
        c=sum-a-b;
        if((a*a)+(b*b)==(c*c)){       //if we get a triplet, it will be the solution.
          sol = (a*b*c);
          found = 1;
          printf("triplet is a:%i, b:%i, c:%i\nsums to %i\nits product is %i\n",a,b,c,(a+b+c),sol);
          break;
        }
      }
    }
  }
  return 0;
}
