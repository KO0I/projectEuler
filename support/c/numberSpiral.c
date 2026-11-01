#include<stdio.h>

int main(){
  int i, j, k, n;
  n=9;  //defines length of sides of square matrix
  int matrix[9][9]={{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};
  

  int center = (n/2);
  k=1; //initial spiral length
  int step = 0; //step length
  int q = 1;

  /*begin spiraling out!*/
  //for(n=1;n<9;n++){ //n is the number of spiral sequences conducted
    step++;
    /*center*/
    matrix[center][center]= q;
    /*left*/
    q++;
    matrix[center][center+1]=q;
    /*down*/
    q++;
    matrix[center+1][center+1]=q;
    /*right*/
    q++;
    matrix[center+1][center]=q;
    q++;
    matrix[center+1][center-1]=q;
    /*up*/
    q++;
    matrix[center][center-1]=q;
    q++;
    matrix[center-1][center-1]=q;

    /*left, second iteration*/


  //}

  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      if((matrix[i][j])<10){
        printf("    ");
        printf("%d",matrix[i][j]);
      }
      else if( ((matrix[i][j])>=10) && ((matrix[i][j])<100) ){
        printf("   ");
        printf("%d",matrix[i][j]);
      }
    }
    printf("\n");
  }
return 0;
}
