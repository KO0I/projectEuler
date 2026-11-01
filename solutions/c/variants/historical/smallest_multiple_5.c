#include<stdio.h>

int main(){
  int done = 0;
  unsigned int i,mult=1;
  unsigned int limit=20;
  while(done==0){
    for(i=1; i<=limit; i++){       //Divide a candidate number by each iteration...
      
      if(mult%i != 0){            //if a number cannot be divided successfully at each iteration...
        i=1;
        mult++;                   //get a new candidate multiple, and reset the for loop.
        }
    }
    printf("smallest multiple that is divisible by each of the numbers from 1 to %i is %i\n",limit,mult);
    done=1; 
    
  }
  return 0;
}
