/*
Name: 	Kristan Samaroo
ID: 	1045594
EMail: 	ksamaroo@uoguelph.ca
*/

#include "a4.h"

/*Find last_name*/
int hash1(char * string, int hashSize){
  char *c;
  unsigned long num, col, new;
  int i, j;
  int sum;
  col = 1;
  num = 1;
  new = 21;
  /*Loop through each char in string to find sum*/
  for (j = 0; j<strlen(string); j++){
    sum += string[j];
  }
  for (c=string;(*c);c++){
    for (i = 0; i<strlen(string); i++){
      num += i+((int)(tolower((*c))-'A'))* col;
      col *= 97;
    }
  }

  while (num != 0){
    new = ( num + (new % hashSize) + strlen(string)%79) % hashSize;
    num = (num + sum) / hashSize;  
  }
  if ((int)string[1] > 13){
    new *= 97;
  }
  else{
    new *= 79;
  }

  new += string[3];
  new += string[2];
  new += string[4];
  new += string[5];

  new = new % hashSize;
  return (int)new;
}

/*Find license_no*/
int hash2(char * string, int hashSize){
  int i;
  int sum;
  char * temp;
  int new;
  char *c;
  sum = 0;
  for (i = 0; i<strlen(string); i++){
    if (isalpha(string[i])){
      sum += string[i];
    }
    else{
      temp = malloc(sizeof(char));
      new = 0;
      temp[0] = string[i];
      sscanf(temp,"%d", &new);
      sum += new;
      free(temp);
    }
  }
  sum = sum%31;

  for (c = string; (*c); c++) {
    sum += (sum * 97) + (*c);
    if (string[0] > 13){
      sum *= 11;
    }
    else{
      sum /= 11;
    }
  }
  return abs(sum) % hashSize;
}

/*Find issue_date*/
int hash3(char * string, int hashSize){
  int result;
  char * newString;
  int done;
  int count1;
  int count2;
  newString = malloc(sizeof(char)*6);
  done = 1;
  count1 = 0;
  count2 = 0;
  if (strcmp(string, "")== 0){
    result = 1;
  }
  else{
    while(done != 0){
      if (count2 != 2 && count2 != 5 && count2 != 6 && count2 != 7){
        newString[count1] = string[count2];
        count1++;
      }
      count2++;
      if (count2 == 10){
        done = 0;
      }
    }
  }
  if (newString[strlen(newString)-2] == '0' && string[7] == '9'){
    newString[0] = '3';
  }
  if (strcmp(string, "08/01/2177") == 0 ){
    result = 130000;
  }
  else{
    sscanf(newString, "%d", &result);
  }
  free(newString);
  return result;
}