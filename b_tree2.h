#include<stdio.h>
#define ORDER 4//number of pointers
#define CLOSE 5

/* structure for an order 4 BTree*/
typedef struct node{
	int keys[ORDER-1];//array of node values
	struct node *p[ORDER];//pointer to the children of the node
	int nodes; //number of keys = order-1
}BTREE;

enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };

//menu for getting choice
int menu(){
	int choice;
	printf("\n\n   MENU     ");
	printf("\n[1] - Insert");
	printf("\n[2] - Search");
	printf("\n[3] - Delete");
	printf("\n[4] - View BTree");
	printf("\n[5] - Exit");
	printf("\nEnter choice here: ");
	scanf("%i",&choice);
	return choice;
}

//function for getting value to be inserted or deleted
int getValue(){
	int x;//variable for value
	
	printf("Enter a key: ");
	scanf("%i",&x);
	
	return x;
}

enum KeyStatus ins(BTREE *ptr, int key, int *upKey,BTREE **newnode){
	BTREE *newPtr, *lastPtr;
	int pos, i, n,splitPos;
	int newKey, lastKey;
	enum KeyStatus value;
	
	//check if value can be inserted to the btree
	if (ptr == NULL){
		*newnode = NULL;
		*upKey = key;
		return InsertIt;
	}
	
	n = ptr->nodes;
	pos = searchPos(key, ptr->keys, n);
	
	//check if value is already in the btree
	if (pos < n && key == ptr->keys[pos])
		return Duplicate;
	
	value = ins(ptr->p[pos], key, &newKey, &newPtr);
	
	if (value != InsertIt)
		return value;
	
	/*If keys in node is less than M-1 where M is order of B tree*/
	if (n < ORDER - 1){
		pos = searchPos(newKey, ptr->keys, n);
		
		/*Shifting the key and pointer right for inserting the new key*/
		for (i=n; i>pos; i--){
			ptr->keys[i] = ptr->keys[i-1];
			ptr->p[i+1] = ptr->p[i];
		}
	
		/*Key is inserted at exact location*/
		ptr->keys[pos] = newKey;
		ptr->p[pos+1] = newPtr;
		++ptr->nodes; /*incrementing the number of keys in node*/
		return Success;
	}/*End of if */
	
	/*If keys in nodes are maximum and position of node to be inserted is
	last*/
	if (pos == ORDER - 1){
		lastKey = newKey;
		lastPtr = newPtr;
	}
	/*If keys in node are maximum and position of node to be inserted is not last*/	
	else{ 
	
		lastKey = ptr->keys[ORDER-2];
		lastPtr = ptr->p[ORDER-1];
		lastPtr = ptr->p[ORDER-1];

		for (i=ORDER-2; i>pos; i--){
			ptr->keys[i] = ptr->keys[i-1];
			ptr->p[i+1] = ptr->p[i];
		}
		
		ptr->keys[pos] = newKey;
		ptr->p[pos+1] = newPtr;
	}
	
	splitPos = (ORDER - 1)/2;
	(*upKey) = ptr->keys[splitPos];

	(*newnode)=malloc(sizeof(struct node));/*Right node after split*/
	ptr->nodes = splitPos; /*No. of keys for left splitted node*/
	(*newnode)->nodes = ORDER-1-splitPos;/*No. of keys for right splitted node*/
	
	for (i=0; i < (*newnode)->nodes; i++){
		(*newnode)->p[i] = ptr->p[i + splitPos + 1];
		
		if(i < (*newnode)->nodes - 1)
			(*newnode)->keys[i] = ptr->keys[i + splitPos + 1];
		else
			(*newnode)->keys[i] = lastKey;
	}
	
	(*newnode)->p[(*newnode)->nodes] = lastPtr;
	return InsertIt;
}/*End of ins()*/

/*Insert a value to the BTREE*/
void insert(BTREE **root, int key){
	struct node *newnode;
	int upKey;
	enum KeyStatus value;
	value = ins(*root, key, &upKey, &newnode);
	if (value == Duplicate)
		printf("Key already available\n");
	if (value == InsertIt){
		BTREE *uproot = *root;
		*root=malloc(sizeof(struct node));
		(*root)->nodes = 1;
		(*root)->keys[0] = upKey;
		(*root)->p[0] = uproot;
		(*root)->p[1] = newnode;
	}/*End of if */
}/*End of insert()*/

//display the BTREE
void display(BTREE *ptr, int blanks){
	int i;
	if(ptr){
		for(i=1;i<=blanks;i++)
			printf(" ");
		
		for (i=0; i < ptr->nodes; i++)
			printf("%d ",ptr->keys[i]);
		
		printf("\n");
		for (i=0; i <= ptr->nodes; i++)
			display(ptr->p[i], blanks+10);
	}/*End of if*/
}/*End of display()*/

//function for searching position of key in BTREE node
int searchPos(int key, int *key_arr, int n){
	int pos=0;
	
	while (pos < n && key > key_arr[pos])
		pos++;
	return pos;
}/*End of searchPos()*/

//search for a certain key
void search(BTREE *root,int key)
{
	int pos, i, n;
	BTREE *ptr = root;
	printf("Search path:\n");
	
	while(ptr){
		n = ptr->nodes;
		
		for (i=0; i < ptr->nodes; i++)
			printf(" %d",ptr->keys[i]);
		printf("\n");
		
		pos = searchPos(key, ptr->keys, n);//search for position of key
		
		if (pos < n && key == ptr->keys[pos]){
			printf("Key %d found in position %d of last dispalyednode\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %d is not available\n",key);
}/*End of search()*/

enum KeyStatus del(BTREE **root,BTREE *ptr, int key){
	int pos, i, pivot, n ,min;
	int *key_arr;
	int nkey;
	enum KeyStatus value;
	BTREE **p,*lptr,*rptr,*qp,*qp1;

	if (ptr == NULL)
		return SearchFailure;
	
	/*Assigns values of node*/
	n=ptr->nodes;
	key_arr = ptr->keys;
	p = ptr->p;
	min = (ORDER - 1)/2;/*Minimum number of keys*/

	pos = searchPos(key, key_arr, n);
	if (p[0] == NULL){
		if (pos == n || key < key_arr[pos])
			return SearchFailure;
	
		/*Shift keys and pointers left*/
		for (i=pos+1; i < n; i++){
			key_arr[i-1] = key_arr[i];
			p[i] = p[i+1];
		}
	
		return --ptr->nodes >= (ptr==*root ? 1 : min) ? Success : LessKeys;
	}/*End of if */

	if (pos < n && key == key_arr[pos]){
		qp = p[pos];
		
		while(1){
			nkey = qp->nodes;
			qp1 = qp->p[nkey];
		
			if (qp1 == NULL) break;
				
			qp = qp1;
		}/*End of while*/

		key_arr[pos] = qp->keys[nkey-1];
		qp->keys[nkey - 1] = key;
	}/*End of if */

	value = del(root, p[pos], key);
	if (value != LessKeys) return value;

	if (pos > 0 && p[pos-1]->nodes > min){
		pivot = pos - 1; /*pivot for left and right node*/
		lptr = p[pivot];
		rptr = p[pos];
		/*Assigns values for right node*/
		rptr->p[rptr->nodes + 1] = rptr->p[rptr->nodes];
		for (i=rptr->nodes; i>0; i--){
			rptr->keys[i] = rptr->keys[i-1];
			rptr->p[i] = rptr->p[i-1];
		}
		rptr->nodes++;
		rptr->keys[0] = key_arr[pivot];
		rptr->p[0] = lptr->p[lptr->nodes];
		key_arr[pivot] = lptr->keys[--lptr->nodes];
		return Success;
	}/*End of if */

	if (pos > min){
		pivot = pos; /*pivot for left and right node*/
		lptr = p[pivot];
		rptr = p[pivot+1];

		/*Assigns values for left node*/
		lptr->keys[lptr->nodes] = key_arr[pivot];
		lptr->p[lptr->nodes + 1] = rptr->p[0];
		key_arr[pivot] = rptr->keys[0];
		lptr->nodes++;
		rptr->nodes--;

		for(i=0; i < rptr->nodes; i++){
			rptr->keys[i] = rptr->keys[i+1];
			rptr->p[i] = rptr->p[i+1];
		}/*End of for*/
		
		rptr->p[rptr->nodes] = rptr->p[rptr->nodes + 1];
		return Success;
	}/*End of if */

	if(pos == n)
		pivot = pos-1;
	else
		pivot = pos;

	lptr = p[pivot];
	rptr = p[pivot+1];

	/*merge right node with left node*/
	lptr->keys[lptr->nodes] = key_arr[pivot];
	lptr->p[lptr->nodes + 1] = rptr->p[0];
	
	for(i=0; i < rptr->nodes; i++){
		lptr->keys[lptr->nodes + 1 + i] = rptr->keys[i];
		lptr->p[lptr->nodes + 2 + i] = rptr->p[i+1];
	}

	lptr->nodes = lptr->nodes + rptr->nodes +1;
	free(rptr); /*Remove right node*/

	for (i=pos+1; i < n; i++){
		key_arr[i-1] = key_arr[i];
		p[i] = p[i+1];
	}

	return (--ptr->nodes >= (ptr == *root ? 1 : min) ? Success : LessKeys);
}/*End of del()*/

//function for deleting a key from the BTree
void DelNode(BTREE **root,int key){
	BTREE *uproot;
	enum KeyStatus value;
	value = del(root,*root,key);
	
	switch (value){
		case SearchFailure: printf("Key %d is not available\n",key);
							break;
		case LessKeys: uproot = *root;
					   *root = (*root)->p[0];
					   free(uproot);
					   break;
	}/*End of switch*/
}/*End of delnode()*/