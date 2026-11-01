#include <stdio.h>
#include <math.h>

//Champernowne's constant

int main(){

  int end = 210;
  int d[end];
  int i,j=1,k=1,n=0;
  int f=0,g=0;
  int secondpow=1,thirdpow=1;/*,fourthpow=1;*/

while(n <= end){

  //under 10
  if(n<=9){//the first 9 digits can be directly added to the array:
    d[n]=n;
    //printf("ones: %i\n",d[n]);
    n++;

  //under 100
  }else if((n>9)&&(n<189)){ 
    d[n]=secondpow;
    d[n+1]=(n-(10*j+f)); 
    if((d[n]==9)&&(d[n+1]==9)){
      secondpow=0;
      f=0;
      j=1;
    }else if((d[n+1]==9)){
      secondpow++;
      j++;
    }
    f++;
    n+=2;

  //under 1000
  }else if((n>=189)&&(n<=end)){
    d[n]=thirdpow;
    d[n+1]=secondpow;
    d[n+2]=(n-(189*j+f));
    if((d[n]==9)&&(d[n+1]==9)&&(d[n+2]==9)){
      thirdpow=0;
      secondpow=0;
      f=0;
      g=0;
      j=1;
      k=1;
    }else if((secondpow==9)&&(d[n+2]==9)){
      thirdpow++;
      secondpow=0;
      f=0;
      j=1;
    }else if((d[n+2]==9)){
      secondpow++;
      j++;
    }
    f++;
    g++;
    n+=3;
  }
}

//print the array!

  printf("\n");
  for(i=0;i<=end;i++){
    printf("%i:%i  \n",i,d[i]);
  }
  printf("\n");

  for(i=0;i<=end;i++){
    printf("%i",d[i]);
  }
printf("\n");
return 0;
}

