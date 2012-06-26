#include<stdio.h>

int array_2d_add(int [][4],int);

int main(void)
{
  int array_2d[4][4];
  int i,j,sum_of_elements,static_sum;
  
  printf("\nEnter 2D Array (4x4)... Enter row by row and use return key between elements...\n\n");
  
  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      scanf("%d",&array_2d[i][j]);
    }
  }
  
  sum_of_elements = array_2d_add(array_2d,4);
  
  printf("Array is:\n\n");
  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      printf("%d",array_2d[i][j]);
      printf("\t");
    }
    printf("\n");
  }
  
  printf("\nSum of all elements of the array is: %d\n\n",sum_of_elements);
  
  return 0;
}

int array_2d_add(int array[][4], int rows)
{
  int i,j,sum = 0;

  for(i=0;i<rows;i++)
  {
    for(j=0;j<4;j++)
    {
      sum = sum + array[i][j];
    }
  }
  return sum;
}