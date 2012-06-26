#include "graph.h"

int mst_kruksal(struct vertex *arr1[],struct vertex *arr2[],int arg_weight,int nodes)
{
	int i,j,k=0,l=0,flag=0,first_flag1=1,count1=0,random_weight,vertex1,vertex2;
	struct vertex *temp1;
	int F[MAX-1][3];                 /* set of edges in min. span. tree */
	int num_edges = 0,edges=0;             /* num of edges in min. span. tree */
	int next_edge = 0;             /* next edge not yet considered */
	int weight = 0;                /* minimal spanning tree weight */
	int a,b,c;          /* counter/placeholder variables */
	
	for(i=0;i<nodes;i++)
	{
		temp1 = arr1[i];
		vertex1 = arr1[i]->vertex_id;
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
				vertex2 = arr1[i]->vertex_id;
				mst_edge_matrix[k][0] = vertex1;          /* first vertex of edge */
				mst_edge_matrix[k][1] = vertex2;          /* second vertex of edge */
				random_weight = (1 + rand() % arg_weight);
				if(random_weight == 0)
				{
					random_weight = arg_weight;
				}
				//printf("\nv1 %d",vertex1);
				//printf("\nv2 %d",vertex2);
				//printf("\nw1 %d",random_weight);
				for(l=0;l<k;l++)
				{
					if((mst_edge_matrix[l][0] == vertex2) && (mst_edge_matrix[l][1] == vertex1))
					{
						//printf("\nmatch");
						mst_edge_matrix[k][2] = mst_edge_matrix[l][2];    /* weight of edge */
						flag = 1;
						break;
					}
				}
				if(flag != 1)
				{
					mst_edge_matrix[k][2] = random_weight;
				}
				k++;
				flag = 0;

			}
			if(arr1[i]->next !=NULL)
			{
				arr1[i] = arr1[i]->next;
			}
		}
		arr1[i] = temp1;
	}
        edges = k;
	/* Display edges */
	printf("\n\n");
	printf("Edges with Weights:\n");
	printf("Edge\tEdge\tWeight\n");
	for (i = 0; i < edges; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf("%d\t", mst_edge_matrix[i][j]);
		}
		printf("\n");
	}

	for (i = edges - 1; i > 0; i--)
	{
		for (j = 0; j < i; j++)
		{
			if (mst_edge_matrix[j][2] > mst_edge_matrix[j+1][2])
			{  
				a = mst_edge_matrix[j][0];
				b = mst_edge_matrix[j][1];
				c = mst_edge_matrix[j][2];
				mst_edge_matrix[j][0] = mst_edge_matrix[j+1][0];
				mst_edge_matrix[j][1] = mst_edge_matrix[j+1][1];
				mst_edge_matrix[j][2] = mst_edge_matrix[j+1][2];
				mst_edge_matrix[j+1][0] = a;
				mst_edge_matrix[j+1][1] = b;
				mst_edge_matrix[j+1][2] = c;   
			}
		}
	}

	/* Display edges */
	/*printf("\n");
	for (i = 0; i < edges; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf(" %3d", mst_edge_matrix[i][j]);
		}
		printf("\n");
	}*/
	/* create n disjoint subsets */
	mst_initial (nodes);

	/* initialize set of edges in min. span. tree to empty */
	for (i = 0; i < nodes - 1; i++)
	{
		for (j = 0; j < 3; j++)
		{
			F[i][j] = -1;            /* '-1' denotes 'empty' */
		}
	}
	//mst_test_univ(nodes);

	/* find minimal spanning tree */
	while (num_edges < nodes - 1)
	{  
		a = mst_edge_matrix[next_edge][0];
		b = mst_edge_matrix[next_edge][1];

		i = mst_find(a);
		j = mst_find(b);

		if (!mst_equal(i, j))
		{  
			mst_merge (i, j);
			F[num_edges][0] = mst_edge_matrix[next_edge][0];
			F[num_edges][1] = mst_edge_matrix[next_edge][1];
			F[num_edges][2] = mst_edge_matrix[next_edge][2];
			num_edges++;

			//mst_test_univ(nodes);
		}

		next_edge++;
	}

	/* display edges comprising minimal spanning tree */
	printf("\n");
	printf("\nMinimal Spanning Tree Edges:\n");
	printf("F = (");
	for (i = 0; i < nodes - 1; i++)
	{  
		printf("(V%d,V%d)", F[i][0], F[i][1]);
		if (i < nodes - 2)
		{
			printf(", ");
		}
		printf("W:%d  ",F[i][2]);
		weight = weight + F[i][2];
	}
	printf(")\n");

	return weight;
}

void mst_makeset (int i)
{  
	mst_U[i] = i;
}

int mst_find (int i)
{  
	int j;

	j = i;
	while (mst_U[j] != j)
	{
		j = mst_U[j];
	}
	return (j);
}

void mst_merge (int p, int q)
{  
	if (p < q)
	{
		mst_U[q] = p;
	}
	else
	{
		mst_U[p] = q;
	}
}

int mst_equal (int p, int q)
{ 
	if (p == q)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

void mst_initial (int n)
{  
	int i;

	for (i = 0; i < n; i++)
	{
		mst_makeset(i);
	}
}

void mst_test_univ (int nodes)
{ 
	int i;

	printf("\nThe disjoint subsets are:\n");
	for (i = 0; i < nodes; i++)
	{
		printf(" %3d", mst_U[i]);
	}
	printf("\n");
}
