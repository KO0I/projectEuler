#include <stdio.h>

//triangle number generator:

int main(){
  unsigned long i,tri=1,j=1;
  for(i=2; i<=500; i++){
    tri+=i;
    j++;
  }
  printf("%lu\n", tri);
  return 0;
}
