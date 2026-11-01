#include <stdio.h>
//For Euler problem 19, this program is used to generate a
//calendar, and also test some properties. Namely, the integer score is the
//number of months that begin with a Sunday in the interval of time between
//year and f. 

int score = 0;    //  number of qualifying months
int year = 1901;  //  starting year
int day = 1;      //  the number associated with the day
int done = 0;     //  A simple Boolean condition
int i,j,leap=0;   //  values that are used for the week, month and leap year
int f=2001;       //  last year to check

//Here, it is not neccessary to have these as strings, but I wanted to have a
//more general calendar generator on the ready.

//January is month[0], February is month[1], etc.
const char *month[12]={"January","February","March","April","May","June","July","August","September","October","November","December"};
//Sunday is week[0], Monday is week[1], etc.
const char *week[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};   

int nextDay(int m){
  if (i<6){
    i++;
  }
  else if (i==6){ //new week!
    i=0;         //Start on Sunday
  }
  day++;
//  This prints the date (good for general usage) :
//printf("%d/%d/%d  (%s)\n",(j+1),day,year, week[i]);
return i;
}

int nextMonth(int n){
  if (year<f){
    day=0;
    if (j<11){
      j++;
    }
    else if (j==11) {  //Happy new year!
      j=0;            //Start on January
      year++;         //increment year
      //And if it increments to the final year, stop!
      if(year==f) (done=1);
    }
    nextDay(i);
    //check to see if the week starts on a Sunday:
    if (i==0){
      score++;
    }
  }
  return j;
}

int main(){
//Remember, 1901 was a Tuesday year, so i=2;
i=2;
j=0;

while(done==0){
  if(j==1) {//February has 28 days, and 29 every four years 
  //Check if year is a leap year only if the month is February:
  //We don't need to check 12 times!
    if(year%4==0){
      leap   =1;
      //  Uncomment to print leap years
      //printf("%d \n",year);
    }
    else leap=0;
    while(day<=28+leap){
      if (day==28+leap){
        nextMonth(j);
        break;
      }
      else nextDay(i);
    }
  }
  else if((j==3)||(j==5)||(j==8)||(j==10)){ //April,June,Sept and Nov have 30 days
    while(day<=30){
      if (day==30){
        nextMonth(j);
        break;
      }
      else nextDay(i);
    }
  }
  else { //if it's any other month, 31 days
    while(day<=31){
      if (day==31){
        nextMonth(j);
        break;
    }
      else nextDay(i);
    }
  }
  }//end of while loop
printf("\n%d\n\n",score);
return 0;
}
