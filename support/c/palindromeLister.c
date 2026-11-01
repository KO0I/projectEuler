#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>

int main(){
  int pal[899];
  int i;
  int j=0;
  for(i=999; i>=100; i--){
    pal[j] = i*1000; //Third Digit (first three)
    pal[j] += (i%10)*100; //"Fourth" digit mirrored
    int temp = i/10; 
    pal[j] += (temp%10)*10; //"Fifth" Digit
    temp = temp/10;
    pal[j] += (temp); //"Sixth" Digit
    j++;
    //Output to a file called palindromes
  }

    const char filename[] = "palindrome";
    FILE *fp = fopen(filename,"w");

    if (fp==NULL){
      printf("failed to open file for write\n");
      return -1;
    }
for (j=0;j<=899;++j){
    fprintf(fp, "%i", pal[j]);
    //fprintf(fp, "\n");
}
    fclose(fp);
  return 0;
}

