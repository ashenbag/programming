#include "graph.h"

void queue_add(int vertex)
{
	if(rear == MAX - 1)
	{
		printf("\nQueue Overflow.");
		exit(0);
	}

	rear++;
	q[rear] = vertex;

	if(front == -1)
		front = 0;
}

int queue_delete()
{
	int vertex_id;

	if(front == -1)
	{
		printf("\nQueue Underflow.");
		exit(0);
	}

	vertex_id = q[front];

	if(front == rear)
		front = rear = -1;
	else
		front++;

	return vertex_id;
}

int queue_isempty()
{
	if(front == -1)
		return TRUE;
	return FALSE;
}
