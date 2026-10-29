#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SIZE 5888890
// champernowne string is up to d_1000000
// where d_n is the position in the decimal sequence

int main(){
  clock_t begin,end;
  begin=clock();
  double time_spent;
  unsigned long i,pos=0, val=0;
  char *d = malloc(SIZE*sizeof(char));
  if(d==NULL) return 1;
  while(pos<SIZE){                  //up to 10
    if(val<10){
      d[pos-1]=val;
      pos+=1; val++;
    }
    if((val>=10) && (val<100)){     //up to 190
      d[pos-1]     = val/10;
      d[pos]   = val%10;
      pos+=2; val++;
    }
    if((val>=100) && (val<1000)){   //up to 2890
      d[pos-1]     = val/100;
      d[pos]   = (val%100)/10;
      d[pos+1]   = val%10;
      pos+=3; val++;
    }
    if((val>=1000) && (val<10000)){ //up to 38890
      d[pos-1]     = val/1000;
      d[pos]   = (val%1000)/100;
      d[pos+1]   = (val%100)/10;
      d[pos+2]   = val%10;
      pos+=4; val++;
    }
    if((val>=10000) && (val<100000)){ //up to 488890
      d[pos-1]     = val/10000;
      d[pos]   = (val%10000)/1000;
      d[pos+1]   = (val%1000)/100;
      d[pos+2]   = (val%100)/10;
      d[pos+3]   = val%10;
      pos+=5; val++;
    }
    if((val>=100000) && (val<1000000)){ //up to 5888890
      d[pos-1]     = val/100000;
      d[pos]   = (val%100000)/10000;
      d[pos+1]   = (val%10000)/1000;
      d[pos+2]   = (val%1000)/100;
      d[pos+3]   = (val%100)/10;
      d[pos+4]   = val%10;
      pos+=6; val++;
    }
  }
  //for(i=1; i<SIZE; i++){
  //  printf("%d",d[i-1]);
  //}
  printf("\n");
  int solution = (d[0]*d[9]*d[99]*d[999]*d[9999]*d[99999]);
  printf("%d\n",d[(10)]);
  //printf("%d*%d*%d*%d*%d*%d\n",d[0],d[9],d[99],d[999],d[9999],d[99999]);
  printf("solution: %d\n",solution);
// free memory
  free(d);
  d=NULL;
  end=clock();
  time_spent = (double)(end-begin) / CLOCKS_PER_SEC;
  printf("%f\n",time_spent);
  return 0;
}
