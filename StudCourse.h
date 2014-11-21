#define M 5

enum Semester {First, Second, Sum};

typedef struct studcourse_record{
	/*Place necessary fields here*/
	int id;
	char cno[11];
	char acadyear[50];	
	char studentno[50];	
    enum Semester semester;	
	
}info;

struct node{

       info studCourseRec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};


struct node *root=NULL;


/*I ADDED THE FF---------------------------------------*/   

info getStudCourseInfo(int *id);
void insertCourseOfferingRec(info y);
enum KeyStatus ins(struct node *r, int *x, int *y, struct node** u, info studCourseRec);
int searchPos(int *x,struct node *key_arr, int n, info studCourseRec);
void display(struct node *root,int);
void DelNode(struct node **root,int x);
enum KeyStatus del(struct node **root,struct node *ptr, int key);
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };
/*----------------------I added the ff-------------------*/
void search_stdno(char *key);
void search_cno(char *key);
void search_acadyear(char *key);
void search_id(int key);
void search_semester(enum Semester semester);

void search_studcourse_data(x){
     int id;
     char stdno[50];
     char acadyear[11];
     char cno[11];
     enum Semester semoffered;
     int so;

     switch(x){
        case 1:
                         printf("Enter course number: \n"); 
                         scanf("%s",cno);
                         search_cno(cno);
                         break;
                                 
        case 2: 
                       printf("Enter stdno: \n"); 
                         scanf("%s",stdno);
                         search_stdno(stdno); 
                         break;
        case 3:
                       printf("Enter acadyear: \n"); 
                         scanf("%s",acadyear);
                         search_acadyear(acadyear); 
                         break;
        case 4:
                       printf("Enter id: \n"); 
                         scanf("%d",&id);
                         search_id(id); 
                         break;

        case 5:         printf("Sem Offered? \n[0] 1st [1] 2nd [2] Sum\t"); 
                         scanf("%d",&so);
                         if(so==0){semoffered = First;}
                          else if(so==1){semoffered = Second;}
                            else if(so==2){semoffered = Sum;}
                         search_semester(semoffered);
                         break;
        default: printf("INVALID");

      }
     
}


void search_id(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->studCourseRec[i].id);
    }
    printf("\n");
    pos = searchPosOfId(key, ptr, n);
    if (pos < n && key == ptr->studCourseRec[pos].id){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
}/*End of search()*/

int searchPosOfId(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->studCourseRec[pos].id ){
    pos++;
  }
  return pos;
  
}

void search_stdno(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studCourseRec[i].studentno);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studCourseRec[pos].studentno)){
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
	while (pos < n && atoi(newkey) > atoi(key_arr->studCourseRec[pos].studentno) ){
		pos++;
	}
	return pos;
	
}

void search_acadyear(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studCourseRec[i].acadyear);
		}
		printf("\n");
		pos = searchPosOfAcadYear(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studCourseRec[pos].acadyear)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/


int searchPosOfAcadYear(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studCourseRec[pos].acadyear) ){
		pos++;
	}
	return pos;
	
}

void search_cno(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studCourseRec[i].cno);
		}
		printf("\n");
		pos = searchPosOfCno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studCourseRec[pos].cno)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/


int searchPosOfCno(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studCourseRec[pos].cno) ){
		pos++;
	}
	return pos;
	
}


void search_semester(enum Semester semoffered){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->studCourseRec[i].semester);
    }
    printf("\n");
    pos = searchPosOfSemOffered(semoffered, ptr, n);
    if (pos < n && semoffered == ptr->studCourseRec[pos].semester){
      printf("Sem Offered: %d found in position %d of last dispalyed node\n",semoffered,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",semoffered);
}/*End of search()*/

int searchPosOfSemOffered(enum Semester semoffered, struct node *key_arr, int n){
  int pos=0;
  enum Semester newkey;
  newkey = semoffered;
  while (pos < n && newkey > key_arr->studCourseRec[pos].semester ){
    pos++;
  }
  return pos;
  
}

info getStudCourseInfo(int *cid){
	info e;
	int i;	
	e.id = *cid;
	(*cid)++;
	//get info from user
    printf("Enter course no: ");
    scanf("%s", e.cno);
	printf("Enter acadyear: ");
    scanf("%s", e.acadyear);
    printf("Enter studentno: ");
    scanf("%s", e.studentno);

	printf("Enter sem offered: [1] 1st [2] 2nd [3] Sum\t ");
	scanf("%d", &i);
	
	switch(i){
		case 1: e.semester = First; break;
		case 2: e.semester = Second; break;
		case 3: e.semester = Sum; break;
		default: printf("Wrong choice.\n");
	}
	
	return e;
}

void display(struct node *ptr, int blanks){
    if(ptr){
		int i,j;
		for(i=1;i<=blanks;i++){
			printf(" ");
		}
		for (i=0; i < ptr->n; i++){
                      printf("| %d %s %s %s ", ptr->studCourseRec[i].id, ptr->studCourseRec[i].cno,ptr->studCourseRec[i].acadyear,ptr->studCourseRec[i].studentno);
                  		
                  	switch(ptr->studCourseRec[i].semester){	
                  		case 0: printf("1st |"); break;
                  		case 1: printf("2nd |"); break;
                  		case 2: printf("3rd |"); break;
                  		
                  	}
		}

					
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			display(ptr->p[i], blanks+10);
		}
	}//End of if
}//End of display()

void insertCourseOfferingRec(info y){
       struct node *newnode;
       int upKey;
       enum KeyStatus value;
       int key;
       
       key = y.id;
       value = ins(root, &key, &upKey, &newnode, y);
      // printf("value inserted 1");
       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;
          root->studCourseRec[0].id = upKey;
          strcpy(root->studCourseRec[0].cno,y.cno);
          strcpy(root->studCourseRec[0].acadyear,y.acadyear);
          strcpy(root->studCourseRec[0].studentno,y.studentno);
          root->studCourseRec[0].semester = y.semester;
          
          root->p[0] = uproot;
          root->p[1] = newnode;
         
       }else{
       }
     
}


enum KeyStatus ins(struct node *ptr, int *key, int *upKey,struct node **newnode,info studCourseRec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    int lastKey;
    char lastacadyear[50];
    char laststudentno[50];
    char lastcno[11];
    
    enum Semester lastsem;
    enum KeyStatus value;
    int newKey;

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			*upKey = *key;
			return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos(key, ptr, n, studCourseRec); //studCourseRec = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
		
		if (pos < n && *key == ptr->studCourseRec[pos].id ){
			return Duplicate;
		}
		value = ins(ptr->p[pos], key, &newKey, &newPtr, studCourseRec);

		if (value != InsertIt){
			return value;
		}

//If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
			pos = searchPos(&newKey, ptr, n, studCourseRec);
			//Shifting the key and pointer right for inserting the new key/
			for (i=n; i>pos; i--){
				ptr->studCourseRec[i] = ptr->studCourseRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			//Key is inserted at exact location
			
			ptr->studCourseRec[pos].id = newKey;
			strcpy(ptr->studCourseRec[pos].cno,studCourseRec.cno);
			strcpy(ptr->studCourseRec[pos].acadyear,studCourseRec.acadyear);
      		strcpy(ptr->studCourseRec[pos].studentno,studCourseRec.studentno);
            ptr->studCourseRec[pos].semester = studCourseRec.semester;
			ptr->p[pos+1] = newPtr;
			++ptr->n; //incrementing the number of keys in node

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			lastKey = newKey;  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
			strcpy(lastcno, studCourseRec.cno);
            strcpy(lastacadyear, studCourseRec.acadyear);
            strcpy(laststudentno, studCourseRec.studentno);
            lastsem=studCourseRec.semester;
            
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			lastKey = ptr->studCourseRec[M-2].id;
			
			strcpy(lastcno, ptr->studCourseRec[M-2].cno);
            strcpy(lastacadyear, ptr->studCourseRec[M-2].acadyear);
            strcpy(laststudentno, ptr->studCourseRec[M-2].studentno);
            lastsem = ptr->studCourseRec[M-2].semester;
            
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->studCourseRec[i] = ptr->studCourseRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			ptr->studCourseRec[pos].id = newKey;
			//added the ff -mgaga
			strcpy(ptr->studCourseRec[pos].cno,studCourseRec.cno);
			strcpy(ptr->studCourseRec[pos].acadyear,studCourseRec.acadyear);
			strcpy(ptr->studCourseRec[pos].studentno, studCourseRec.studentno);
            ptr->studCourseRec[pos].semester = studCourseRec.semester;
            
			ptr->p[pos+1] = newPtr;
		}
		splitPos = (M - 1)/2;
        *upKey = ptr->studCourseRec[splitPos].id;	
	    
        (*newnode)=malloc(sizeof(struct node));//Right node after split
		ptr->n = splitPos; //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
			if(i < (*newnode)->n-1){
				(*newnode)->studCourseRec[i].id = ptr->studCourseRec[i + splitPos + 1].id;
				strcpy((*newnode)->studCourseRec[i].cno, ptr->studCourseRec[i + splitPos + 1].cno);
				strcpy((*newnode)->studCourseRec[i].acadyear, ptr->studCourseRec[i + splitPos + 1].acadyear);
				strcpy((*newnode)->studCourseRec[i].studentno, ptr->studCourseRec[i + splitPos + 1].studentno);
				(*newnode)->studCourseRec[i].semester = ptr->studCourseRec[i + splitPos + 1].semester;
			}else{
				(*newnode)->studCourseRec[i].id = lastKey;
				strcpy( (*newnode)->studCourseRec[i].cno,lastcno );
				strcpy( (*newnode)->studCourseRec[i].acadyear,lastacadyear );
				strcpy( (*newnode)->studCourseRec[i].studentno,laststudentno );
				
				(*newnode)->studCourseRec[i].semester = lastsem;
			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of ins()



int searchPos(int *key, struct node *key_arr, int n, info studCourseRec){
	int pos=0;
	int newkey;
	newkey = *key;
	while (pos < n && newkey > key_arr->studCourseRec[pos].id ){
		pos++;
	}
	return pos;
	
}//End of searchPos()


int searchPosOfcno(int key, struct node *key_arr, int n){
	int pos=0;
	int newkey;
	newkey = key;
	while (pos < n && newkey > key_arr->studCourseRec[pos].id ){
		pos++;
	}
	return pos;
	
}

void search(int key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %d",ptr->studCourseRec[i].id);
		}
		printf("\n");
		pos = searchPosOfcno(key, ptr, n);
		if (pos < n && key == ptr->studCourseRec[pos].id){
			printf("Course Offering id: %d found in position %d of last displayed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %d is not available\n",key);
}/*End of search()*/



void DelNode(struct node **root, int key){
	struct node *uproot;
	enum KeyStatus value;
	
	value = del(root,*root,key);
	
	switch (value){
		case SearchFailure:
			printf("Key %d is not available\n",key);
			break;
		case LessKeys: 
			uproot = *root;
                        *root = (*root)->p[0];
                         free(uproot);
                         break;
	}//End of switch
}//End of delnode()

enum KeyStatus del(struct node **root,struct node *ptr, int key){
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

	pos = searchPosOfcno(key, knode, n);	

	if(p[0] == NULL){
                if (pos == n || key < knode->studCourseRec[pos].id)
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->studCourseRec[i-1] = knode->studCourseRec[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && key == knode->studCourseRec[pos].id ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->studCourseRec[pos] = qp->studCourseRec[nkey-1];
                //qp->keys[nkey - 1] = key;
		qp->studCourseRec[nkey-1].id = key;
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
                        rptr->studCourseRec[i] = rptr->studCourseRec[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->studCourseRec[0] = knode->studCourseRec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->studCourseRec[pivot] = lptr->studCourseRec[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
                lptr->studCourseRec[lptr->n] = knode->studCourseRec[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                knode->studCourseRec[pivot] = rptr->studCourseRec[0];
                lptr->n++;
                rptr->n--;

                for(i=0; i < rptr->n; i++){
                        rptr->studCourseRec[i] = rptr->studCourseRec[i+1];
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
        lptr->studCourseRec[lptr->n] = knode->studCourseRec[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
       
        for(i=0; i < rptr->n; i++){
                lptr->studCourseRec[lptr->n + 1 + i] = rptr->studCourseRec[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
        }

        lptr->n = lptr->n + rptr->n +1;
        free(rptr); /*Remove right node*/

        for (i=pos+1; i < n; i++){
                knode->studCourseRec[i-1] = knode->studCourseRec[i];
                p[i] = p[i+1];
        }

        return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);


}//End of del()
