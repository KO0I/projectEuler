#include <stdio.h>
#define NUMB 120 // 9 * 120 = 1080 total digits.
#define SIZE 1000000000 // 9 digit numbers 
int main() { 
  int i = 0; 
  int j = 0; 
  int bigNum1[NUMB]; 
  int bigNum2[NUMB]; 
  int bigNum3[NUMB]; 
  int counter = 0; 
  for (i = 0; i < NUMB; i++) { 
    bigNum1[i] = 0; 
    bigNum2[i] = 0; 
    bigNum3[i] = 0; 
  } 
  bigNum1[0] = 1; 
  bigNum2[0] = 1; 
  counter = 2; 
  i = 0; 
  
  while (i == 0) { 
    counter++; 
    for (j = 0; j < NUMB; j++) { 
      bigNum3[j] = bigNum2[j] + bigNum1[j];
    } 
    for (j = 0; j < NUMB-1; j++) { 
      while (bigNum3[j] >= SIZE) { 
        bigNum3[j] -= SIZE; bigNum3[j+1]++;
      }
    }
    if (bigNum3[111] >= 1) break; 
    for (j = 0; j < NUMB; j++) { 
      bigNum1[j] = bigNum2[j]; bigNum2[j] = bigNum3[j];
    }
  }
  printf("\n"); 
  printf("P025 answer = %u\n", counter);
}
