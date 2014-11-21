#define M 5

    
typedef struct studhist_record{
	/*Place necessary fields here*/
	char stdno[11];
	char description[50];	
	char action[50];	
    	char dateFiled[10];
   	char dateResolved[10];
	
}info;

struct node{

       info studHistRec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};


struct node *root=NULL;
/*I ADDED THE FF---------------------------------------*/   

void insertStudent();
void insertstudHistRec(info y);
enum KeyStatus ins(struct node *r, char *x, char *y, struct node** u, info studHistRec);
int searchPos(char *x,struct node *key_arr, int n, info studHistRec);
int searchPosOfstdno(char *x,struct node *key_arr, int n);
void display(struct node *root,int);
void DelNode(struct node **root,char *x);
enum KeyStatus del(struct node **root,struct node *ptr, char *key);
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };	
void search_stdno(char *x);
void search_description(char *x);
void search_action(char *x);
void search_dateFiled(char *x);
void search_dateResolved(char *x);

void search_studhist_data(x){
     char stdno[11];
     char description[50];
     char action[11];
     char dateFiled[50];
     char dateResolved[50];

     switch(x){
        case 1:
                         printf("Enter student no: \n"); 
                         scanf("%s",stdno);
                         search_stdno(stdno);
                         break;
                                 
        case 2: 
                       printf("Enter description: \n"); 
                         scanf("%s",description);
                         search_description(description); 
                         break;
        case 3:
                       printf("Enter action: \n"); 
                         scanf("%s",action);
                         search_action(action); 
                         break;
        case 4:
                       printf("Enter date filed: \n"); 
                         scanf("%s", dateFiled);
                         search_dateFiled(dateFiled); 
                         break;

       case 5:
                       printf("Enter date resolved: \n"); 
                         scanf("%s", dateResolved);
                         search_dateResolved(dateResolved); 
                         break;

        default: printf("INVALID");

      }
     
}

void search_stdno(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studHistRec[i].stdno);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studHistRec[pos].stdno)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_description(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studHistRec[i].description);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studHistRec[pos].description)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_action(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studHistRec[i].action);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studHistRec[pos].action)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_dateFiled(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studHistRec[i].dateFiled);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studHistRec[pos].dateFiled)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_dateResolved(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studHistRec[i].dateResolved);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studHistRec[pos].dateResolved)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

int searchPosOfStdno(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studHistRec[pos].stdno) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfDesc(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studHistRec[pos].description) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfAction(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studHistRec[pos].action) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfDateFiled(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studHistRec[pos].dateFiled) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfDateResolved(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studHistRec[pos].dateResolved) ){
		pos++;
	}
	return pos;
	
}

/*----------------------I added the ff-------------------*/
info getStudHistInfo(){
	info e;
	int i;
	
	//get info from user
    printf("Enter student no: ");
    scanf("%s", e.stdno);
	printf("Enter description: ");
    scanf("%s", e.description);
    printf("Enter action: ");
    scanf("%s", e.action);
    printf("Enter date filed:  ");
    scanf("%s", e.dateFiled);
    printf("Enter date resolved: ");
	scanf("%s", e.dateResolved);

	
	return e;
}


void display(struct node *ptr, int blanks){
    if(ptr){
		int i,j;
		for(i=1;i<=blanks;i++){
			printf(" ");
		}
		for (i=0; i < ptr->n; i++){
                      printf("| %s %s %s %s %s |", ptr->studHistRec[i].stdno,ptr->studHistRec[i].description,ptr->studHistRec[i].action,ptr->studHistRec[i].dateFiled,ptr->studHistRec[i].dateResolved);
		}

					
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			display(ptr->p[i], blanks+10);
		}
	}//End of if
}//End of display()


void insertCourseRec(info y){
       struct node *newnode;
       char upKey[11];
       enum KeyStatus value;
       char key[11];
       
       strcpy(key,y.stdno);
       value = ins(root, key, upKey, &newnode, y);
      // printf("value inserted 1");
       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;
          //root->keys[0] = upKey;
          strcpy(root->studHistRec[0].stdno,upKey);
          strcpy(root->studHistRec[0].description,y.description);
          strcpy(root->studHistRec[0].action,y.action);
          strcpy(root->studHistRec[0].dateFiled, y.dateFiled);
          strcpy(root->studHistRec[0].dateResolved, y.dateResolved);
          
          root->p[0] = uproot;
          root->p[1] = newnode;
         
       }else{
       }
     
}


enum KeyStatus ins(struct node *ptr, char *key, char *upKey,struct node **newnode,info studHistRec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    char lastKey[11];
    char lastdescription[50];
    char lastaction[50];
    char lastdateFiled[10];
    char lastdateResolved[10];
    enum KeyStatus value;
    char newKey[11];

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			//upKey = key;
			strcpy(upKey,key);
			return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos(key, ptr, n, studHistRec); //studHistRec = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
		
		if (pos < n && strcmp(key, ptr->studHistRec[pos].stdno)==0  ){
			return Duplicate;
		}
		value = ins(ptr->p[pos], key, newKey, &newPtr, studHistRec);

		if (value != InsertIt){
			return value;
		}

//If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
			pos = searchPos(newKey, ptr, n, studHistRec);
			//Shifting the key and pointer right for inserting the new key/
			for (i=n; i>pos; i--){
				ptr->studHistRec[i] = ptr->studHistRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			//Key is inserted at exact location
			strcpy(ptr->studHistRec[pos].stdno,newKey);
			strcpy(ptr->studHistRec[pos].description,studHistRec.description);
      		strcpy(ptr->studHistRec[pos].action,studHistRec.action);
			strcpy(ptr->studHistRec[pos].dateFiled,studHistRec.dateFiled);
            strcpy(ptr->studHistRec[pos].dateResolved, studHistRec.dateResolved);
			ptr->p[pos+1] = newPtr;
			++ptr->n; //incrementing the number of keys in node

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			strcpy(lastKey, newKey);  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
			strcpy(lastdescription, studHistRec.description);
            strcpy(lastaction, studHistRec.action);
            strcpy(lastdateFiled, studHistRec.dateFiled);
            strcpy(lastdateResolved, studHistRec.dateResolved);
            
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			strcpy(lastKey,ptr->studHistRec[M-2].stdno);
			//added the following -mgaga
            strcpy(lastdescription,ptr->studHistRec[M-2].description);
            strcpy(lastaction,ptr->studHistRec[M-2].action);
            strcpy(lastdateFiled, ptr->studHistRec[M-2].dateFiled);
            strcpy(lastdateResolved, ptr->studHistRec[M-2].dateResolved);
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->studHistRec[i] = ptr->studHistRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			strcpy(ptr->studHistRec[pos].stdno,newKey);
			//added the ff -mgaga
			strcpy(ptr->studHistRec[pos].description,studHistRec.description);
			strcpy(ptr->studHistRec[pos].action,studHistRec.action);
			strcpy(ptr->studHistRec[pos].dateFiled, studHistRec.dateFiled);
            strcpy(ptr->studHistRec[pos].dateResolved, studHistRec.dateResolved);
            
			ptr->p[pos+1] = newPtr;
		}
		splitPos = (M - 1)/2;
	//	(*upKey) = ptr->keys[splitPos];
	 //   printf("SPLITPOS: %d",splitPos);
        strcpy(upKey,ptr->studHistRec[splitPos].stdno);
	  //  printf("UPKEY: %s, %s",upKey, ptr->studHistRec[splitPos].stdno);		
	    
        (*newnode)=malloc(sizeof(struct node));//Right node after split
		ptr->n = splitPos; //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
			if(i < (*newnode)->n-1){
				strcpy((*newnode)->studHistRec[i].stdno, ptr->studHistRec[i + splitPos + 1].stdno);
				strcpy((*newnode)->studHistRec[i].description, ptr->studHistRec[i + splitPos + 1].description);
				strcpy((*newnode)->studHistRec[i].action, ptr->studHistRec[i + splitPos + 1].action);
				strcpy((*newnode)->studHistRec[i].dateFiled, ptr->studHistRec[i + splitPos + 1].dateFiled);
				strcpy((*newnode)->studHistRec[i].dateResolved, ptr->studHistRec[i + splitPos + 1].dateResolved);
			}else{
				strcpy( (*newnode)->studHistRec[i].stdno,lastKey );
				strcpy( (*newnode)->studHistRec[i].description,lastdescription );
				strcpy( (*newnode)->studHistRec[i].action,lastaction );
				
				strcpy((*newnode)->studHistRec[i].dateResolved, lastdateResolved);
				strcpy((*newnode)->studHistRec[i].dateFiled, lastdateFiled);
			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of ins()

int searchPos(char *key, struct node *key_arr, int n, info studHistRec){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studHistRec[pos].stdno) ){
		pos++;
	}
	return pos;
	
}//End of searchPos()

int searchPosOfstdno(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studHistRec[pos].stdno) ){
		pos++;
	}
	return pos;
	
}

void search(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studHistRec[i].stdno);
		}
		printf("\n");
		pos = searchPosOfstdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studHistRec[pos].stdno)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/


void DelNode(struct node **root, char *key){
	struct node *uproot;
	enum KeyStatus value;
	
	value = del(root,*root,key);
	
	switch (value){
		case SearchFailure:
			printf("Key %s is not available\n",key);
			break;
		case LessKeys: 
			uproot = *root;
                        *root = (*root)->p[0];
                         free(uproot);
                         break;
	}//End of switch
}//End of delnode()

enum KeyStatus del(struct node **root,struct node *ptr, char *key){
        int pos, i, pivot, n ,min;
        int *key_arr;
	struct node *knode;
        int nkey;
        enum KeyStatus value;
        struct node **p,*lptr,*rptr,*qp,*qp1;

        if (ptr == NULL)
                return SearchFailure;
       
        /*Assigns values of node*/
        n=ptr->n;
        knode = ptr;
        p = ptr->p;
        min = (M-1)/2;/*Minimum number of keys*/

	pos = searchPosOfstdno(key, knode, n);	

	if(p[0] == NULL){
                if (pos == n || (atoi(key) < atoi(knode->studHistRec[pos].stdno)))
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->studHistRec[i-1] = knode->studHistRec[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && (atoi(key) == atoi(knode->studHistRec[pos].stdno)) ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->studHistRec[pos] = qp->studHistRec[nkey-1];
                //qp->keys[nkey - 1] = key;
		strcpy(qp->studHistRec[nkey-1].stdno,key);
        }//End of if

        value = del(root, p[pos], key);
        if (value != LessKeys) return value;

        if (pos > 0 && p[pos-1]->n > min){
                pivot = pos - 1; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pos];
                /*Assigns values for right node*/
                rptr->p[rptr->n + 1] = rptr->p[rptr->n];
                for (i=rptr->n; i>0; i--){
                        rptr->studHistRec[i] = rptr->studHistRec[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->studHistRec[0] = knode->studHistRec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->studHistRec[pivot] = lptr->studHistRec[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
                lptr->studHistRec[lptr->n] = knode->studHistRec[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                knode->studHistRec[pivot] = rptr->studHistRec[0];
                lptr->n++;
                rptr->n--;

                for(i=0; i < rptr->n; i++){
                        rptr->studHistRec[i] = rptr->studHistRec[i+1];
                        rptr->p[i] = rptr->p[i+1];
                }/*End of for*/
               
                rptr->p[rptr->n] = rptr->p[rptr->n + 1];
                return Success;
        }/*End of if */

        if(pos == n)
                pivot = pos-1;
        else
                pivot = pos;

        lptr = p[pivot];
        rptr = p[pivot+1];


        /*merge right node with left node*/
        lptr->studHistRec[lptr->n] = knode->studHistRec[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
       
        for(i=0; i < rptr->n; i++){
                lptr->studHistRec[lptr->n + 1 + i] = rptr->studHistRec[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
        }

        lptr->n = lptr->n + rptr->n +1;
        free(rptr); /*Remove right node*/

        for (i=pos+1; i < n; i++){
                knode->studHistRec[i-1] = knode->studHistRec[i];
                p[i] = p[i+1];
        }

        return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);


}//End of del()


