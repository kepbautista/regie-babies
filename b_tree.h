/*
	Notes:
		* A value of 0 means an empty value...
*/
#define ORDER 4//number of pointers
#define NODES 3//number of nodes
#define CLOSE 4

/* structure for an order 4 BTree*/
typedef struct node{
	int x[NODES];//array of node values
	struct node *p[ORDER];//pointer to the children of the node
	struct node *parent;//pointer to the parent of the node
	int nodes;
}BTREE;

//menu for getting choice
int menu(){
	int choice;
	printf("\n\n   MENU     ");
	printf("\n[1] - Insert");
	printf("\n[2] - Search");
	printf("\n[3] - View BTree");
	printf("\n[4] - Exit");
	printf("\nEnter choice here: ");
	scanf("%i",&choice);
	return choice;
}

//function for getting value to be inserted or deleted
int getValue(){
	int x;//variable for value
	
	printf("Value: ");
	scanf("%i",&x);
	
	return x;
}

//inserts node pointed by temp to the BTREE
void insertNode(BTREE **root,BTREE *temp){
	//insert a new node
	if(*root==NULL) *root=temp;
	else{
	
	}
}

void insertValue(BTREE **root,int x){
	int i;
	BTREE *temp;

	//if root does not have a value yet, create a new node
	if(*root==NULL){
		temp = (BTREE*)malloc(sizeof(BTREE));
		temp->x[0] = x;
		temp->nodes = 1;
		
		//initialize node pointers to NULL
		for(i=0;i<ORDER;i++) temp->p[i]=NULL;
		
		temp->parent=NULL;//initialize parent of node to null
		insertNode(root,temp);
	}
	//current node is full
	else if(isFull(*root)){
		
		
	}
	//current node is not empty and not full
	else (*root)->x[(*root)->nodes++] = x;
}

//returns a pointer to a node with value x
BTREE *search(BTREE *root,int x){
	if(root!=NULL){
		if(x < root->x[0])
			root=search(root->p[0]);
		else if(x > root->x[2])
			root=search(root->p[3]);
		else if((x > root->x[0]) && (x < root->x[1]))
			root=search(root->p[1]);
		else if((x > root->x[1]) && (x < root->x[2]))
			root=search(root->p[2]);
		else return root;
	}
}

//check if a node is full
int isFull(BTREE *root){
	if(root->nodes==NODES) return 1;//node is full
	else return 0;//node is not yet full
}

//print the BTREE
void printBTREE(BTREE *root,int tabs){
	int i;
	
	if(root!=NULL){
		printBTREE(root->p[0],tabs+1);
		printBTREE(root->p[1],tabs+1);
		printf("%2i %2i %2i\n",root->x[0],root->x[1],root->x[2]);
		printBTREE(root->p[2],tabs+1);
		printBTREE(root->p[3],tabs+1);
	}
}
