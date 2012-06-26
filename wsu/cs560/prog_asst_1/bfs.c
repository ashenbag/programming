#include "graph.h"

int bfs(int v, struct vertex **p,int nodes)
{
	struct vertex *u;
	int visited[MAX],i;
	int connect_count=0;
	for(i=0;i<MAX;i++)
	{
		visited[i] = FALSE;
	}
	visited[v] = TRUE;
	queue_add(v);

	while(queue_isempty() == FALSE)
	{
		v = queue_delete();
		u = *(p + v );

		while(u != NULL)
		{
			if(visited [ u -> vertex_id ] == FALSE)
			{
				queue_add(u -> vertex_id);
				visited [ u -> vertex_id] = TRUE;
				connect_count++;
			}
			u = u -> next;
		}
	}
	if(connect_count == (nodes -1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
