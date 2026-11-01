#include<stdio.h>
#include<assert.h>

//Amicable numbers
//an Amicable pair is defined as d(a)=b and d(b)=a where d(n) returns the sum
//of proper divisors of n

unsigned int d(unsigned int n){       //finds divisors of n
  unsigned int i,w;        
  w=1;              //the sum of proper divisors is at least 1 (iff prime)

  for(i=2; i<n; i++){
    if(n%i==0){     //proper divisor, add it to the sum, j
      w+=i;
    }
  }
  return w;
}

unsigned int amicable(unsigned int n){
  unsigned int a,b,j,tempsum=0,prevsum=0,sum=0;
  for(j=200; j<n; j++){     //check if j generates an amicable number 
    a=d(j);
    b=d(a);
    
    //the number j yeilds an amicable number pair
    //the condition of (b>a) removes duplicates 
    if(( b>a )&&( a!=b )&&( d(a)==b )&&( d(b)==a )){
      tempsum=(a+b);

      if( prevsum != tempsum ){
          printf("a: %u  b: %u \n",a,b);
          sum += tempsum; 
          prevsum = tempsum;
      }
    }
  }
  return sum;
}
  

int main(){
  unsigned int n=10000;
  printf("sum of all amicable numbers below %u\n  sum: %u\n",n,amicable(n));
  return 0;
}
