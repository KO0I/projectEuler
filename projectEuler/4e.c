#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>

#define LINE_LENGTH 7

int main(){
  clock_t start = clock();
  const char filename[] = "palindrome";
  FILE *fp = fopen(filename,"r");

  if (fp==NULL){
    printf("failed to read the file.\n");
    return -1;
  }
  char line[LINE_LENGTH];
  int max=100001;                 //smallest possible 6-digit palindrome.
  int a,b;
  int q;

  while(fgets(line, LINE_LENGTH, fp) != NULL){
    int temp = atoi(line);          //temp is now a palindrome on the list
//    printf("%d ", temp);
    for(a=100; a<=999; a++){
      if(temp%a==0){              //if a can be divided into that palindrome...
        q = temp/a;
//        printf("%d divides into %d,yields %d\n",a,temp,q);
        for(b=100; b<=999; b++){  //consider another value, b...
          int prod=(a*b);
          if(temp==prod){
//            printf("B: %d, A*B: %d\n",b,prod);
                                  //such that the product of a & b is that palindrome...
            if(temp>max){         //compare that palindrome to max...
              max=temp;
            }
          }
        }
      }
    }
  }
  printf("Max is: %i\n",max);
  clock_t stop = clock();
  double runtime = ((double) (stop-start))/CLOCKS_PER_SEC;
  printf("Runtime: %f\n",runtime);
  fclose(fp);
  return 0;
} 
