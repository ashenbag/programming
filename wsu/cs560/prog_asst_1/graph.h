#ifndef _GRAPH_H_
#define _GRAPH_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define MAX 100
#define MAX_WEIGHT 1000

struct vertex
{
        int vertex_id;
        struct vertex *next;
};

int mst_edge_matrix[(MAX*(MAX-1))][3];
int mst_N[MAX];
int mst_U[MAX];
int q[MAX];
int front, rear;

void graph_display(struct vertex *[],int);
void graph_prune(struct vertex *[],struct vertex *[],int);

void ll_display(struct vertex *);
int ll_count(struct vertex *);
void ll_delete(struct vertex **, int);

void queue_add(int);
int queue_delete();
int queue_isempty();

int bfs(int, struct vertex **,int);
struct vertex * getvertex_write(int);

int mst_kruksal(struct vertex *[],struct vertex *[],int,int);
void mst_makeset(int);
int mst_find(int);
void mst_merge(int, int);
int mst_equal(int, int);
void mst_initial(int);
void mst_test_univ(int);
#endif //Header File Macro End
