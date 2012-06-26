#include<stdio.h>

struct date
{
  int date;
  int month;
  int year;
};
int main(void)
{
  struct date today,tomorrow;
  
  printf("\nEnter today's date:");
  scanf("%d",&today.date);
  printf("\nEnter current month:");
  scanf("%d",&today.month);
  printf("\nEnter current year:");
  scanf("%d",&today.year);
  
  printf("Today is %d.%d.%d",today.date,today.month,today.year);
  
  if(today.date < 28)
  {
    tomorrow.date = today.date + 1;
    tomorrow.month = today.month;
    tomorrow.year = today.year;
  }
  else if(today.month == 2)
  {
    if(today.date == 28 && (today.year % 4) == 0))
    {
      tomorrow.date = today.date + 1;
      tomorrow.month = today.month + 1;
    }
    else if(today.date == 28 && (today.year % 4) != 0)
  
  printf("\n\nTomorrow is %d.%d.%d",tomorrow.date,tomorrow.month,tomorrow.year);
  
  printf("\n");
  
  return 0;
}
