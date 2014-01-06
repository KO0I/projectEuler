// take the names in names.txt and do the following:
// alphabatize the list
// next, use the ascii values for each character to find the
// "alphabetical value" A=1, B=2, C=3, D=4, E=5, etc.
// so each name  has a name score
// the answer is the sum of all 15,000 name scores
// watch out! file access is absolute!
#include <stdlib.h>
#include <stdio.h>
int i = 0;
//FILE *sortedNames;
int n;
char *name;

int main(){
  //try to open the file
  FILE *unsortedNames = fopen("/home/patrick/projectEuler/names.txt", "r");
  if (unsortedNames == NULL){
    printf("Unable to open file! \n");
    fclose(unsortedNames);
    return 1;
  }
  name = malloc(6000*sizeof(char));
  if(name[i-1]=='"'){
  while((name[i-1] != EOF) && (name[i-1]!='"')){
    name[i++]=(char)fgetc(unsortedNames);
  }
  }
name[i] = '\0';
puts(name);
fclose(unsortedNames);
return 0;
}
