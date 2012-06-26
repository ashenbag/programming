#include<stdio.h>

typedef float (*t_FunPtr)(float,float);

float Add(float,float);
float Sub(float,float);
float Mul(float,float);
float Div(float,float);

float Switch(float,float,char);
float SwitchFunPtr(float,float,float(*)(float,float));

float (*GetFunPtr(char))(float,float);

t_FunPtr t_GetFunPtr(char);


int main()
{
	float a,b,result;
	char operator;
	
	float (*funptr)(float,float);

	fflush(stdin);
	printf("\nEnter Number 1:");
	scanf("%f",&a);
	printf("Enter Number 2:");
	scanf("%f",&b);
	printf("Enter Operator:");
	scanf("%c",&operator);
	scanf("%c",&operator);

	result = Switch(a,b,operator);

	printf("\nRegular Function call: Result: %f",result);

	printf("\nMethod1:");
	funptr = GetFunPtr(operator);
	if(funptr == NULL){
		printf("\n");
		return 0;
	}
	result = SwitchFunPtr(a,b,funptr);
	printf("\nFunction Pointer call: Result: %f",result);

	printf("\nMethod2:");
	funptr = t_GetFunPtr(operator);
	if(funptr == NULL){
		printf("\n");
		return 0;
	}
	result = SwitchFunPtr(a,b,funptr);
	printf("\nFunction Pointer call: Result: %f",result);
	
	printf("\n");

	return 0;
}

float Add(float a, float b)
{
	return (a+b);
}
float Sub(float a, float b)
{
	return (a-b);
}
float Mul(float a, float b)
{
	return (a*b);
}
float Div(float a, float b)
{
	if(b == 0){
		printf("\nNo Divide by 0.");
		return 0;
	}
	return (a/b);
}

float Switch(float a, float b, char operator)
{
	float result;

	switch(operator)
	{
		case '+':
			result = Add(a,b);
			break;
		case '-':
			result = Sub(a,b);
			break;
		case '*':
			result = Mul(a,b);
			break;
		case '/':
			result = Div(a,b);
			break;
		default:
			printf("\nInvalid Operator");
			result = 0;
			break;

	}
	return result;
}

float SwitchFunPtr(float a, float b, float(*funptr)(float,float))
{
	return ((*funptr)(a,b));
}

float (*GetFunPtr(char operator))(float,float)
{
	if(operator == '+'){
		return &Add;
	}
	else if(operator == '-'){
		return &Sub;
	}
	else if(operator == '*'){
		return &Mul;
	}
	else if(operator == '/'){
		return &Div;
	}
	else{
		printf("\nInvalid Operator");
		return NULL;
	}
}

t_FunPtr t_GetFunPtr(char operator)
{
	if(operator == '+'){
		return &Add;
	}
	else if(operator == '-'){
		return &Sub;
	}
	else if(operator == '*'){
		return &Mul;
	}
	else if(operator == '/'){
		return &Div;
	}
	else{
		printf("\nInvalid Operator");
		return NULL;
	}
}
