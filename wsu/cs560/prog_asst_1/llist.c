#include "graph.h"

void ll_display(struct vertex *q)
{
	printf("\n");

	/* traverse the entire linked list */
	while(q != NULL)
	{
		printf("%d ", q -> vertex_id);
		q = q -> next;
	}
}

/* deletes the specified vertex from the linked list */
void ll_delete(struct vertex **q, int num)
{
	struct vertex *old, *temp;

	temp = *q;

	while(temp != NULL)
	{
		if(temp -> vertex_id == num)
		{
			/* if vertex to be deleted is the first vertex in the linked list */
			if(temp == *q)
				*q = temp -> next;

			/* deletes the intermediate vertexs in the linked list */
			else
				old -> next = temp -> next;

			/* free the memory occupied by the vertex */
			free(temp);
			return;
		}

		/* traverse the linked list till the last vertex is reached */
		else
		{
			old = temp;  /* old points to the previous vertex */
			temp = temp -> next;  /* go to the next vertex */
		}
	}

	printf("\nElement %d not found", num);
}

/* counts the number of nodes present in the linked list */
int ll_count(struct vertex * q)
{
	int c = 0;

	/* traverse the entire linked list */
	while(q != NULL)
	{
		q = q -> next;
		c++;
	}

	return c;
}
