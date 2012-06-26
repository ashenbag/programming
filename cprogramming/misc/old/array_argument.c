#include<stdio.h>

void print_my_name(char []);
void print_my_id(int []);

int main(void)
{
  char name[]="Anbarasan";
  
  int id[5]={1,5,6,9,8};
  
  print_my_name(name);
  print_my_id(id);
  
  return 0;
}

void print_my_name(char name[])
{
  printf("\n Name is:%s \n",name);
}

void print_my_id(int id[])
{
  int i;
  printf("Size of ID is %d",sizeof(id));
  printf("\n ID is:");
  for(i = 0;i<5/*(sizeof(id))*/;i++)
  {
    printf("%d",id[i]);
  }
  printf("\n \n");
}
