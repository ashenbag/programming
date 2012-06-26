#include<stdio.h>

int main(void)
{
  char word[20];
  
  word[0] = 'a';
  word[1] = 'n';
  word[2] = 'b';
  word[3] = 'a';
  word[4] = 'r';
  word[6] = 'p';
  word[10] = 'd';
  word[11] = '\0';
  
  printf("\nWord[] is %s \n",word);
  
  return 0;
}
