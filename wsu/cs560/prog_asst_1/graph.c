#include "graph.h"

int main(int argc, char *argv[])
{
	struct vertex *arr[MAX];
	struct vertex *v[MAX];
	int nodes,weight;
	double alpha;
	int i,j,k,is_connected=1,minimum_weight=0;
	float random_number;
	char *end_ptr;
	long int long_int;

	if(argc!=4)
	{
		printf("\nInvalid Input. Format of input is ./graph <vertices> <alpha> <weight>\n\n");
		return 0;
	}
	long_int = strtol(argv[1],&end_ptr,10);
	if((!((atoi(argv[1]) >= 2) && (atoi(argv[1]) <= MAX))) || (end_ptr == argv[1]) || (*end_ptr != '\0'))
	{
		printf("\nPlease enter number of vertices between 2 - %d.\n\n",MAX);
		return 0;
	}
	if((!((atof(argv[2]) >= 0) && (atoi(argv[2]) <= 1))))
	{
		printf("\nPlease enter probability between 0 - 1.\n\n");
		return 0;
	}
	if((!((atoi(argv[3]) >= 1) && (atoi(argv[3]) <= MAX_WEIGHT))) || (end_ptr == argv[3]) || (*end_ptr != '\0'))
	{
		printf("\nPlease enter weight value between 1 - %d.\n\n",MAX_WEIGHT);
		return 0;
	}
	
	nodes = atoi(argv[1]);
	alpha = atof(argv[2]);
	weight = atoi(argv[3]);
	
	srand ( (unsigned)time ( NULL ) );

	/* Create Edges based on input alpha */
	for(i=0;i<nodes;i++)
	{
		v[0] = getvertex_write(i);
		v[0]->next = NULL;
		arr[i] = v[0];
		k=0;
		for(j=0;j<nodes;j++)
		{
			if(i==j)
			{
				continue;
			}
			random_number = ((1 + rand() % 100)/100.0);
			/* More the edges more the edges */
			if(random_number < alpha || alpha == 1)
			{
				v[k] -> next = v[k+1] = getvertex_write(j);
				v[k+1] -> next = NULL;
				k++;
			}
		}
	}

	printf("\nGenerated Graph:");
	graph_display(arr,nodes);


	/* Prune graph */
	graph_prune(arr,arr,nodes);


	printf("\nPruned Graph:");
	graph_display(arr,nodes);


	/* Check fully connectivity */
	/* Call BFS for all nodes */
	for(i=0;i<nodes;i++)
	{
		front = rear = -1;
		is_connected = bfs(i,arr,nodes);
		if(is_connected == 0)
		{
			printf("\nBFS Result: Graph is not fully connected. ");
			printf("\n\nNo Spanning Tree.");
			break;
		}
	}
	if(is_connected == 1)
	{
		printf("\nBFS Result: Graph is fully connected. ");


		/* Assign weight and calculate minimum weight */
		minimum_weight = mst_kruksal(arr,arr,weight,nodes);


		printf("\nThe Minimum weight of Minimum Spanning Tree calculated using Kruskal Algorithm is %d", minimum_weight);
	}

	printf("\n\n");

	return 0;
}

struct vertex * getvertex_write(int val)
{
	struct vertex *newvertex;
	newvertex =(struct vertex *) malloc(sizeof(struct vertex));
	newvertex -> vertex_id = val;
	return newvertex;
}

void graph_display(struct vertex *arr[],int nodes)
{
	int i;

	for(i=0;i<nodes;i++)
	{
		ll_display(arr[i]);
	}
	printf("\n");
}

void graph_prune(struct vertex *arr1[],struct vertex *arr2[],int nodes)
{
	int i,j,k,vertex_id,vertex_id_test,found_flag=0,first_flag1=1,first_flag2=1,count1=0,count2=0;
	struct vertex *temp1,*temp2;
	
	for(i=0;i<nodes;i++)
	{
		temp1 = arr1[i];
		vertex_id_test = arr1[i]->vertex_id;
		//printf("\nroot vertex: %d",arr1[i]->vertex_id);
		count1 = ll_count(arr1[i]);
		first_flag1 = 1;
		for(j=0;j<count1;j++)
		{
			if(first_flag1 == 1)
			{
				first_flag1 = 0;
			}
			else
			{
				vertex_id = arr1[i]->vertex_id;
				//printf("\n checking for vertex: %d",arr1[i]->vertex_id);
				//printf("\n checking for array vertex: %d",vertex_id);
				count2 = ll_count(arr2[vertex_id]);
				first_flag2 = 1;
				found_flag = 0;
				temp2 = arr2[vertex_id];
				for(k=0;k<count2;k++)
				{
					if(first_flag2 == 1)
					{
						first_flag2 = 0;
					}
					else
					{
						//printf("\n checking against vertex: %d",arr2[vertex_id]->vertex_id);
						if(arr2[vertex_id]->vertex_id == vertex_id_test)
						{
							//printf("\n match found.");
							found_flag = 1;
							break;
						}
					}
					if(arr2[vertex_id]->next != NULL)
					{
						arr2[vertex_id] = arr2[vertex_id]->next; 
					}
				}
				if(found_flag != 1)
				{
					//printf("\n calling delete");
					//printf("\n deleting %d",vertex_id);
					ll_delete(&temp1,vertex_id);
				}
				arr2[vertex_id] = temp2;
			}
			if(arr1[i]->next !=NULL)
			{
				arr1[i] = arr1[i]->next;
			}
		}
		arr1[i] = temp1;
	}

}
