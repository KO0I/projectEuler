//Digit factorials:
//
//145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.
//
//Find the sum of all numbers which are equal to the sum of the factorial of
//their digits.
//
//Note: as 1! = 1 and 2! = 2 are not sums they are not included.

#include <stdio.h>
#include <math.h>

int fact(int n){
  int i,f=1;
  if(n<0){
    return 0;
  }else if(n==0){
    return 1;
  }else if(n>0){
    for (i=1; i<=n; i++){
      f*=i;
    }
  }
  return f;
}

int main(){
  int j,sum=0;
  //It's logical to assume that no numbers beyond 99999 can have a factorial
  //sum equal to the number that generated that sum  
  for(j=100; j<=99999; j++){
    int t=0,a=-1,b=-1,c=-1,d=-1,e=-1;
    
    //the following conditionals filter out leading zeros
    a = (j%10);
      t+=fact(a);
    if(j>10){
      b = ((j%100)-j%10)/10;
      t+=fact(b);
    }else b=0;
    if(j>100){
      c = (((j%1000)-j%100)/100);
      t+=fact(c);
    }else c=0;
    if(j>1000){
      d = (((j%10000)-j%1000)/1000);
      t+=fact(d);
    }else d=0;
    if(j>10000){
      e = (((j%100000)-j%10000)/10000);
      t+=fact(e);
    }else e=0;

    //printf("a:%i, b:%i, c:%i, d:%i, e:%i,     t:%i, j:%i\n",a,b,c,d,e,t,j);
    if(t==j){
      printf("curious number:%i\n",j);
      sum+=j;
    }
  }
  printf("sum of all 'curious numbers':%i\n",sum);
  return 0;
}
