#define ORDER 5
#define EXIT_PROGRAM 3

typedef struct{
	char stdno[10];
	char fname[50], lname[50];
	char degree[10];
}student_tuple;

typedef struct btree_node{
	//number of records per node is equal to ORDER-1
	student_tuple x[ORDER-1];
	
	//number of POINTERS is equal to number of ORDER
	struct btree_node *node_ptr[ORDER];
	
	int size;//number of records stored in the btree
	int maxsize;//max number of records in the btree
	
	int ptr_size;//number of node ptrs used
}BTREE;

//create a new BTREE node
BTREE* createBTREENode(int maxsize){
	int i;

	BTREE *t;
	t=(BTREE*)malloc(sizeof(BTREE));
	t->maxsize=maxsize;
	t->size=0;
	t->ptr_size=0;
	
	//initialize all node pointers to NULL
	for(i=0;i<ORDER;i++)
		t->node_ptr[i] = NULL;
	
	return t;
}

//get student information
student_tuple getStudentValue(){
	student_tuple value;
	
	printf("Enter student number: ");
	scanf("%s",value.stdno);
	printf("Enter first name: ");
	scanf("%s",value.fname);
	printf("Enter last name: ");
	scanf("%s",value.lname);
	printf("Enter degree: ");
	scanf("%s",value.degree);
	
	return value;
}

//store value inside a node
void insertValue(BTREE *node, student_tuple x){
	node->x[node->size] = x;
	node->size++;
}

//insert a new value
void insertNode(BTREE **node,student_tuple x){

	//value to be inserted is 1st value
	if(*node==NULL){
		*node=createBTREENode(ORDER-1);
		insertValue(*node,x);
	}
	//current node is not yet full
	else if((*node)->size<(*node)->maxsize) insertValue(*node,x);
	//current node is full
	else{
		(*node)->node_ptr[(*node)->ptr_size]=createBTREENode(ORDER);
		insertNode(&((*node)->node_ptr[(*node)->ptr_size]),x);
		(*node)->ptr_size++;//increment no. of used pointers by 1	
	}
}

void printNodeValues(BTREE *node){
	int i;

	printf("Current Node\n");
	for(i=0;i<node->size;i++)
		printf("%s %s %s %s\n",node->x[i].stdno,node->x[i].fname,node->x[i].lname,node->x[i].degree);
}

void printBTREE(BTREE *root){
	int i;
	
	printNodeValues(root);
	
	for(i=0;i<root->ptr_size;i++)
		printNodeValues(root->node_ptr[i]);
}

//print menu and get user choice
int printMenu(){
	int choice;
	printf("\n[1] - Add a value\n");
	printf("[2] - View values\n");
	printf("[3] - Exit\n");
	printf("Enter choice: ");
	scanf("%d",&choice);
	return choice;
}
