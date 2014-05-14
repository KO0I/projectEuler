// take the names in names.txt and do the following:
// alphabatize the list
// next, use the ascii values for each character to find the
// "alphabetical value" A=1, B=2, C=3, D=4, E=5, etc.
// so each name  has a name score
// the answer is the sum of all 15,000 name scores
//
// Boy, this is sure ugly, but it works!
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define LISTSIZE 2
//#define NUMBER_OF_NAMES 3
#define NUMBER_OF_NAMES 5163
#define LENGTH_OF_NAMES 12
int i=0, j=0, k=0, quotationMarks=0;
//FILE *sortedNames;
int n, ch;
char names[ NUMBER_OF_NAMES ][ LENGTH_OF_NAMES+1 ];

int main(){
  //try to open the file
  //FILE *unsortedNames = fopen("/home/patrick/projectEuler/names.txt", "rt");
  FILE *unsortedNames = fopen("names.txt", "rt");
  if (unsortedNames == NULL){
    printf("Unable to open file! \n");
    fclose(unsortedNames);
    return 1;
  }

  /* if the file can be read, read in the names until EOF
     so as we go through the file, we are looking for " marks. If we find a quotation
     mark, we take the characters and record them until we find another quotation mark. We need to ignore commas; 
     the only thing that is relevant is the quotation marks. In the end (once we reach EOF), we should check that 
     the quotationMarks variable is an even number (which should be incremented every time a quotation mark is 
     found.

     After all the names are recorded, we need to sort them into alphabetical order. This can be done using any 
     number of sorting algorithms.

     Once in alphabetical order, we need to score each name in the sorted list of names. Because these are all 
     ASCII characters, we can convert the character symbols to score values, and then take a sum of each. 
     Then we multiply each "name score" by its position in the list.

     Finally, we take the sum of all the name scores and print it out.
     */
  //take names as they are ordered in the file, add to the string array
  for(i=0;j<=NUMBER_OF_NAMES; i++){
    fseek(unsortedNames, i, SEEK_SET); //seek a character
    ch = getc(unsortedNames);          //grab a character
    //skip if it's a quotation mark
    if(ch == '"'){//skip!
      quotationMarks++;
      if(j!=(NUMBER_OF_NAMES-1)){
        i++;
        fseek(unsortedNames, i, SEEK_SET); //seek a character
        ch = getc(unsortedNames);        //grab a character
      }
      else break; // if we're at the end, stop (unless you want a segfault)
    }
    if(ch == ','){//skip!
      quotationMarks++; // a comma is always followed by a quote mark
      i+=2;
      fseek(unsortedNames, i, SEEK_SET); //seek a character
      ch = getc(unsortedNames);          //grab a character
      if(quotationMarks%2!=0){ 
        //printf("\n"); //end of name; print newline
        names[j][k]='\0'; //print null terminator
        j++; //new name
        k=0;
        if(j>=NUMBER_OF_NAMES) break;
      }
    }
    //printf("%c",ch); //print the character
    names[j][k]=ch;//add the character to the string for the right name
    k++;
  }
  /* 
  // print out what we've stored in memory 
  for(j=0; j<NUMBER_OF_NAMES; j++){
  for(k=0; k<=LENGTH_OF_NAMES; k++){
  printf("%c",names[j][k]); //print a name
  }
//if(j%10==0) printf("\n");
//else printf(" ");
printf("\n");
}
printf("\n");
*/

// sorting the names with a bubble sort
// Bubble sort compares two entries at a time
// The high values "float" to the top and the low
// values "sink" to the bottom.
int tryagain=0, sorted=0;
int a=0;
char temp;

// Bubble sort - only by first letter
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=0;
    if(names[j][k] != names[j+1][k]){ //not a match
      if(names[j][k] > names[j+1][k]){
        tryagain=1;
        //swap the two entire names!
        for(a=0; a<LENGTH_OF_NAMES; a++){
          if((names[j][a]==0)&&(names[j+1][a]==0)) break;
          temp = names[j+1][a];
          names[j+1][a]=names[j][a];
          names[j][a]=temp;
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}

// Bubble sort - only by second letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=1;
    if(names[j][k-1] == names[j+1][k-1]){ //first letters must match
      if(names[j][k] != names[j+1][k]){ //second letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}

// Bubble sort - only by third letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=2;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])){ //first/second letters must match
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}

// Bubble sort - only by fourth letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=3;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])&&(names[j][k-2] == names[j+1][k-2])){
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}
// Bubble sort - only by fifth letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=4;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])&&(names[j][k-2] == names[j+1][k-2]&&(names[j][k-3]==names[j+1][k-3]))){
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}
// Bubble sort - only by sixth letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=5;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])&&(names[j][k-2] == names[j+1][k-2]&&(names[j][k-3]==names[j+1][k-3])&&(names[j][k-4]==names[j+1][k-4]))){
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}
// Bubble sort - only by seventh letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=6;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])&&(names[j][k-2] == names[j+1][k-2]&&(names[j][k-3]==names[j+1][k-3])&&(names[j][k-4]==names[j+1][k-4])&&(names[j][k-5]==names[j+1][k-5]))){
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}

// Bubble sort - only by eigth letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=7;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])&&(names[j][k-2] == names[j+1][k-2]&&(names[j][k-3]==names[j+1][k-3])&&(names[j][k-4]==names[j+1][k-4])&&(names[j][k-5]==names[j+1][k-5])&&(names[j][k-6]==names[j+1][k-6]))){
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}
// Bubble sort - only by ninth letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=8;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])&&(names[j][k-2] == names[j+1][k-2]&&(names[j][k-3]==names[j+1][k-3])&&(names[j][k-4]==names[j+1][k-4])&&(names[j][k-5]==names[j+1][k-5])&&(names[j][k-6]==names[j+1][k-6])&&(names[j][k-7]==names[j+1][k-7]))){
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}
// Bubble sort - only by tenth letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=9;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])&&(names[j][k-2] == names[j+1][k-2]&&(names[j][k-3]==names[j+1][k-3])&&(names[j][k-4]==names[j+1][k-4])&&(names[j][k-5]==names[j+1][k-5])&&(names[j][k-6]==names[j+1][k-6])&&(names[j][k-7]==names[j+1][k-7])&&(names[j][k-8]==names[j+1][k-8]))){
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}
// Bubble sort - only by eleventh letter
sorted=0;
while(!sorted){
  for(j=0; j<(NUMBER_OF_NAMES-1); j++){
    k=10;
    if((names[j][k-1] == names[j+1][k-1])&&(names[j][k-2] == names[j+1][k-2])&&(names[j][k-2] == names[j+1][k-2]&&(names[j][k-3]==names[j+1][k-3])&&(names[j][k-4]==names[j+1][k-4])&&(names[j][k-5]==names[j+1][k-5])&&(names[j][k-6]==names[j+1][k-6])&&(names[j][k-7]==names[j+1][k-7])&&(names[j][k-8]==names[j+1][k-8])&&(names[j][k-9]==names[j][k-9]))){
      if(names[j][k] != names[j+1][k]){ //third letter not a match
        if(names[j][k] > names[j+1][k]){
          tryagain=1;
          for(a=0; a<LENGTH_OF_NAMES; a++){
            if((names[j][a]==0)&&(names[j+1][a]==0)) break;
            temp = names[j+1][a];
            names[j+1][a]=names[j][a];
            names[j][a]=temp;
          }
        }
      }
    }
  }
  n++;
  if(!tryagain) sorted++; // sorted!
  else tryagain=0;          // tryagain
}

//with all of the names alphabetized, we can now set up the scoring
// all we have to do is take any character and subtract 64 from it
// then add each letter together, then multiply it by its position in the
// list, and then, in a seperate loop, sum all those sums into a grand sum!
int sum[NUMBER_OF_NAMES],digit=0,grandsum=0;
for(j=0; j<NUMBER_OF_NAMES; j++){
  sum[j]=0;
  for(i=0; i<LENGTH_OF_NAMES; i++){
//    printf("%c = %d",names[j][i],names[j][i]);
    digit=names[j][i];
    if(digit>64) digit-=64;
//    printf(" => %d\n",digit);
    sum[j]+=digit;
  }
  sum[j]*=(j+1);
printf("factor: %d, sum*factor = %d\n",(j+1),sum[j]);
}
//Calculating the "grand" sum
for(j=0; j<NUMBER_OF_NAMES; j++){
  grandsum+=sum[j];
}
printf("GRANDSUM = %d\n", grandsum);
//}
// print out to a file what we've sorted in memory 
/*FILE *sortedNames;
sortedNames = fopen("sortedNames.txt", "w");
for(j=0; j<NUMBER_OF_NAMES; j++){
  for(k=0; k<=LENGTH_OF_NAMES; k++){
    if(names[j][k]!=0)
      fprintf(sortedNames,"%c",names[j][k]); //print a name
  }
  //if(j%10==0) printf("\n");
  //else printf(" ");
  fprintf(sortedNames,"\n");
}*/
//printf("\n");
//printf("Iterations to get sorted: %d\n", n);
fclose(unsortedNames);
//fclose(sortedNames);
return 0;
}

