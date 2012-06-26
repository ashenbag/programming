#include<stdio.h>
#include<string.h>

int main(void)
{
  char string[10];
  
  printf("\nEnter a string:");
  scanf("%s",string);
  
  strupr(string);
  
  printf("In upper case is %s",string);
  
  return 0;
}
