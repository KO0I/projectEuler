#include <stdio.h>

int main(int argc, char ** argv) {
  if(argc != 2) {
    return -1;
  }
  int n;
  if(sscanf(argv[1], "%d", &n)==0){
    return -1;
  }
 
  int i,sum=0;
  for (i =1; i<=n; i++)
    sum +=i;
  printf("sum: %d\n",sum);
  return 0;
}
