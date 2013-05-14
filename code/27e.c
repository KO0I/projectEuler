//  Quadratic Primes
#include <stdio.h>

int p,a,b=0,n,score,win,z=1000,highscore=1;

//  Evaluates the quadratic:
//  p(n) = n^2 + an + b
int pr(int *x,int *a_, int *b_){
  p = ( (*x) * (*x) ) + ( *a_ * (*x) ) + *b_;
  return p;
}

int main(){
//  Consider a range of a and b
  for(a=-62;a<=-60;a++){
    b=90;
    while(b<=100){
      if(isPrime(b)==0){
      //  Test n in range of 200
        for(n=0;n<=100;n++){
          pr(&n,&a,&b);  
          //  If the evaluation is prime, increment score
          if(isPrime(p)==0){
            printf("Testing: n^2 + %d n + %d \n",a,b);
            score++;
            //  Test to see if the score beats the highscore
            if(score>highscore){ 
              highscore=score;
              //  Since this is a notable value, save the product of a and b
              win = a*b;
            }
          }
          else if(isPrime(p)==1){
            //since we're looking for consecutive primes, the first sign of a
            //non-prime evaluation merits moving on to the next case
            
            //reset score
            score = 0;
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

