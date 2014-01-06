// p is the perimeter of a right triangle with integral length sides {a,b,c}.
// for p=120, there are 3 solutions: {20,48,52}, {24,45,51}, {30,40,50}
// for p<= 1000, which value of p gives the most solutions?
#include <stdio.h>
#include <math.h>
#include <time.h>
unsigned int score=0, highscore=0, p=12;

int testvalid(a,b,c){
    if((pow(a,2)+pow(b,2))==(pow(c,2))){ //is it right?
      if((a+b+c)==p){        //is perimeter right?
      //if(!score) printf("{%d,%d,%d}",a,b,c);
      //else printf(", {%d,%d,%d}",a,b,c);
      return 0;   //solution!
    }
  }
    else return 1;
}

int main(){
  clock_t begin,end;
  double time_spent;
  begin = clock();
  unsigned int a,b,c;
  while(p<=1000){
    a=(p>>3)+5;  //a is shortest leg
    b=(p>>2)-1;   // b is other leg
    c=(p>>2);   // c is hypotenuse
    for(a=(p>>3); a<(p/3); a++){
      for(b=((p>>2)-1); b<c; b++){
        for(c=(p>>2); c<(p>>1);c++){
          //printf("(%d,%d,%d)\n",a,b,c);
          if((c>a)&&(c>b)&&(a<b)&&(b<c)){
            if(!testvalid(a,b,c)){
              score++;
              c++;    //no duplicates
            }
          }
        }
      }
    }
    if(score>highscore){
      highscore=score;
      //printf("score for p=%d => %d \n", p,highscore);
    }
    if(p>420) p+=8; //even faster!
    else p+=4;      //really important optimization!
    score=0;
  }
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("score: %d\n",highscore);
  printf("%f\n",time_spent);
  return 0;
}
