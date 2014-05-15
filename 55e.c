#include <stdio.h> // printf

unsigned long long reverse(unsigned long long n){//take n, reverse it and then add together
  unsigned long long x=0;
  for(;n;x=x*10+n%10,n/=10);return x;
}

int isPalindrome(unsigned long long candidate){
  int result;
  // from the simple definition of a palindrome
  if(candidate == (reverse(candidate))) result=1;
  else result = 0;
  return result;
}

int main(){
  int score=0,i,lychrel=1;
  unsigned long long a,b,test;
  for(a=1; a<10001; a++){//test numbers under 10000
    lychrel=1;
    test=a; // don't change candidate number since it is index, too
    for(i=0; i<50; i++){//give up after 50 attempts
      //printf("%llu + ",test);
      b=reverse(test); //reverse a
      //printf("%llu = ",b);
      test+=b;
      //printf("%llu\n",test);
      if(isPalindrome(test)){
        lychrel=0;
        break;
      }
    }
    if(lychrel){
      //printf("%llu is possibly a Lychrel number!\n",a);
      score++;
    }
    //else printf(" So, %llu is NOT a Lychrel number\n",a);
  }
  printf("%d Lychrel numbers found\n",score);
  return 0;
}
