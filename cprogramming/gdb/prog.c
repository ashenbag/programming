#include<stdlib.h>

nop()
{
}
func1(int arg)
{
	func2(arg+1);
}
func2(int arg)
{
	int val = arg;
	char *ptr = NULL;

	if(val>0)
		*ptr = 0;
	else
	{
		ptr = (char *)malloc(10);
		nop();

		*ptr =0;
	}
}

main()
{
	func1(1);
}

