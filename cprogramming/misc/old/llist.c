#include<stdio.h>
#include<stdlib.h>

struct node
{
	int index;
	int data;
	struct node *next;
};

/* Function prototypes */
int add_at_start(struct node *,int);
int display_ll(struct node *);
int add_at_end(struct node *,int);
int modify_at_start(struct node *,int);
int modify_at_end(struct node *,int);
int add_after(struct node *,int,int);
int delete_node(struct node *,int);
int modify_node(struct node *,int,int);

int main(void)
{
	/* One structure used in the program */
	struct node llist;
	int rc,i;

	printf("\nAdding at the start of the list...");

	rc = add_at_start(&llist,53);
	if(rc)
	{
		printf("\nAdding at the start of the list not successful...");
		exit(0);
	}
	printf("\nAdding at the start of the list successful...");

	printf("\nDisplaying the list...");
	rc = display_ll(&llist);

	printf("\nAdding at the end...");
	rc = add_at_end(&llist,103);

	printf("\nDisplaying the list...");
	rc = display_ll(&llist);

	printf("\nModifying the start of the list...");
	rc = modify_at_start(&llist,43);

	printf("\nDisplaying the list...");
	rc = display_ll(&llist);

	printf("\nModifying the end of the list...");
	rc = modify_at_end(&llist,203);

	for(i=0;i<5;i++)
	{
		rc = add_at_end(&llist,(i+99));
	}
	
	printf("\nDisplaying the list...");
	rc = display_ll(&llist);
	
	for(i=0;i<3;i++)
	{
		rc = add_at_end(&llist,(i+150));
	}

	printf("\nDisplaying the list...");
	rc = display_ll(&llist);
	
	printf("\nInserting after node 3...");
	rc = add_after(&llist,3,504);
	
	printf("\nDisplaying the list...");
	rc = display_ll(&llist);
	
	printf("\nModifying node 4...");
	rc = modify_node(&llist,4,555);
	
	printf("\nDisplaying the list...");
	rc = display_ll(&llist);

	printf("\nDeleting Node 5...");
	rc = delete_node(&llist,5);

	printf("\nDisplaying the list...");
	rc = display_ll(&llist);

	for(i=0;i<3;i++)
	{
		rc = add_at_end(&llist,(i+342));
	}
	
	printf("\nDisplaying the list...");
	rc = display_ll(&llist);
	
	printf("\nInserting after node 1...");
	rc = add_after(&llist,1,35);
	
	printf("\nDisplaying the list...");
	rc = display_ll(&llist);

	printf("\n\n");

	return 0;
}

/* Called at the start of the progrm only to add data in the begining of the list */
int add_at_start(struct node *head,int new_data)
{
	head->index = 0;
	head->data = new_data;
	head->next = NULL;

	return 0;
}

/* Traverses through the list and displays data */
int display_ll(struct node *head)
{
	struct node *current_node;
	current_node = head;
	while(current_node->next!=NULL)
	{
		printf("\n%d: %d",current_node->index,current_node->data);
		current_node = current_node->next;
	}
	printf("\n%d: %d",current_node->index,current_node->data);

	return 0;
}

/* Traverses to the end of the node, creates a new node and appends the new node to the end
 * and makes the new node as the end of the node */
int add_at_end(struct node *head,int new_data)
{
	struct node *current_node,*temp_node;
	current_node = head;

	while(current_node->next!=NULL)
	{
		current_node = current_node->next;
	}
	temp_node = (struct node *)malloc(sizeof(struct node));
	temp_node->index = (current_node->index) + 1;
	temp_node->data = new_data;
	temp_node->next = NULL;

	current_node->next = temp_node;

	return 0;
}

/* Modifies the data only in first node */
int modify_at_start(struct node *head,int new_data)
{
	head->data = new_data;

	return 0;
}

/* Traverses to the end node and modifies the data only */
int modify_at_end(struct node *head,int new_data)
{
	struct node *current_node;
	current_node = head;

	while(current_node->next!=NULL)
	{
		current_node = current_node->next;
	}
	current_node->data = new_data;

	return 0;
}

/* Traverses to the node with appropriate index and adds a node after that */
int add_after(struct node *head,int index,int new_data)
{
	struct node *current_node,*temp_node;
	current_node = head;
	while(current_node->next!=NULL)
	{
		if(index == current_node->index)
		{
			break;
		}
		current_node = current_node->next;
	}

	temp_node = (struct node *)malloc(sizeof(struct node));
	temp_node->data = new_data;
	temp_node->next = current_node->next;
	
	current_node->next = temp_node;
	
	while(current_node->next!=NULL)
	{
		(current_node->next)->index = (current_node->index) + 1;
		current_node = current_node->next;
	}

	return 0;
}

/* Deletes a node */
int delete_node(struct node *head,int index)
{
	struct node *current_node,*temp_node;
	current_node = head;
	while(current_node->next!=NULL)
	{
		if(index == current_node->index)
		{
			break;
		}
		temp_node = current_node;
		current_node = current_node->next;
	}

	temp_node->next = current_node->next;
	free(current_node);
	current_node = temp_node;
	
	while(current_node->next!=NULL)
	{
		(current_node->next)->index = (current_node->index) + 1;
		current_node = current_node->next;
	}

	return 0;
}

/* Modifies a node */
int modify_node(struct node *head,int index,int new_data)
{
	struct node *current_node;
	current_node = head;
	while(current_node->next!=NULL)
	{
		if(index == current_node->index)
		{
			current_node->data = new_data;
			break;
		}
		current_node = current_node->next;
	}
	return 0;
}
