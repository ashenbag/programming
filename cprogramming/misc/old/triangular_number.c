#include<stdio.h>

int main(void)
{
  int i,number,triangular_number=0;
  
  printf("\n Enter a number:");
  scanf("%d",&number);
  
  for(i=number;i>0;i--)
  {
    triangular_number = triangular_number + i;
  }
  
  printf("\n So the triangular number is %d. \n",triangular_number);
  
  return 0;
}
