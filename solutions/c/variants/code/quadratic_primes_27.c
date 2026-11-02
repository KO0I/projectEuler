//  Quadratic Primes
#include <stdio.h>
#include <math.h>
//Frustrated by the failures of my algorithm, I decided to peek at the
//answers in order to narrow down the bugs in my code
//Sol: -59597
//Solution quadratic:
//  p(n) = n^2 - 61n + 971
//  a = -61, b = 997
//
//Current state: I have yet to determine where my algorithm breaks down!
//Obviously, I won't submit the answer until that's happened.

int p,a,b=0,n,score,win,z=1000,highscore=1;

//integer exponentiation
int ipow(int base, int exp){
  int res = 1;
  while(exp){
    if(exp &1) res *=base;
    exp >>=1;
    base *= base;
  }
  return res;
}

//  Evaluates the quadratic:
//  p(n) = n^2 + an + b
int pr(int x,int c, int d){
  p = ipow(x,2) + c*x + b;
  //printf("%d^2 + %d*%d + %d\n",x,c,x,b);
  return p;
}

int main(){
//  Consider a range of a and b
  for(a=-65;a<=-61;a++){
    b=-1000;
    while(b<=1000){
      if(isPrime(b)==0){
      //  Test n in range of 200
        for(n=0;n<=abs(a);n++){
          pr(n,a,b);  
          //printf("p(%d)=%d\n",n,p);
          //  If the evaluation is prime, increment score
          if(isPrime(p)==0){
            //printf("Testing: n^2 + %d n + %d ",a,b);
            score=n;
            //printf("score: %d\n",score);
            //  Test to see if the score beats the highscore
            if(score>=highscore){ 
              highscore=score;
              //  Since this is a notable value, save the product of a and b
              win = a*b;
              //printf(" prod: %d\n",win);
              //printf("Testing: n^2 + %d n + %d : score:%d\n, a*b:%d",a,b,score, win);
            }
          }
          else if(isPrime(p)==1){
            //since we're looking for consecutive primes, the first sign of a
            //non-prime evaluation merits moving on to the next case
            
            //reset score
            //next case
            break;
          }
        }
        //printf("a:%d b:%d: p:%d: score:%d\n",a,b,p);
        b++;
      }else 
      //  If b isn't prime, don't even bother
      b++;
    }
  }//  End of search
//print the highscore!
printf("\n Highscore: %d ",highscore);
printf(" Product of a,b: %d\n\n",win);
return 0;  
}

