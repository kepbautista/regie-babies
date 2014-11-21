#define M 5

    
enum Semester {First, Second, Sum};
typedef struct course_record{
	/*Place necessary fields here*/
	char course_no[11];
	char course_title[50];	
	char course_desc[50];	
    int has_lab;
   	int no_of_units;
    enum Semester semoffered;	
	
}info;

struct node{

       info course_record[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};


struct node *root=NULL;  

void insertCourseRec(info y);
enum KeyStatus ins(struct node *r, char *x, char *y, struct node** u, info course_record);
int searchPos(char *x,struct node *key_arr, int n, info course_record);
int searchPosOfCno(char *x,struct node *key_arr, int n);
void display(struct node *root,int);
void DelNode(struct node **root,char *x);
enum KeyStatus del(struct node **root,struct node *ptr, char *key);
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };
void search_by_course_no(char *key);
void search_by_course_title(char *key);
void search_by_course_desc(char *key);
void search_by_has_lab(int key);
void search_by_no_of_units(int key);
void search_sem_offered(enum Semester semoffered);

void search_course_data(x){
     char course_num[11];
     char course_title[50];
     char course_desc[11];
     int has_lab;
     int no_of_units;
     int so;
     enum Semester semoffered;

     switch(x){
        case 1:
                         printf("Enter course number: \n"); 
                         scanf("%s",course_num);
                         search_by_course_no(course_num);
                         break;
                                 
        case 2:			 printf("Enter course title: \n"); 
                         scanf("%s",course_title);
                         search_by_course_title(course_title); 
                         break;
        case 3:
                       printf("Enter course description: \n"); 
                         scanf("%s",course_desc);
                         search_by_course_desc(course_desc); 
                         break;
        case 4:
                       printf("Enter no of units: \n"); 
                         scanf("%d",&no_of_units);
                         search_by_no_of_units(no_of_units); 
                         break;

        case 5:         printf("Course with lab? \n[1] Yes [2] No \t"); 
                         scanf("%d",&has_lab);
                         search_by_has_lab(has_lab);
                         break;

        case 6:         printf("Sem Offered? \n[0] 1st [1] 2nd [2] Sum\t"); 
                         scanf("%d",&so);
                         if(so==0){semoffered = First;}
                          else if(so==1){semoffered = Second;}
                            else if(so==2){semoffered = Sum;}
                         search_sem_offered(semoffered);
                         break;
        default: printf("INVALID");

      }
     
}

/*-------------------------------------------SEARCH COURSE_NO-------------------------------------------*/

void search_by_course_no(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->course_record[i].course_no);
    }
    printf("\n");
    pos = search_pos_of_course_no(key, ptr, n);
    if (pos < n && atoi(key) == atoi(ptr->course_record[pos].course_no)){
      printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int search_pos_of_course_no(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && atoi(newkey) > atoi(key_arr->course_record[pos].course_no) ){
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCH COURSE_TITLE-------------------------------------------*/

void search_by_course_title(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->course_record[i].course_title);
    }
    printf("\n");
    pos = search_pos_of_course_title(key, ptr, n);
    if (pos < n && atoi(key) == atoi(ptr->course_record[pos].course_title)){
      printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int search_pos_of_course_title(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && atoi(newkey) > atoi(key_arr->course_record[pos].course_title) ){
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCH COURSE_DESC-------------------------------------------*/

void search_by_course_desc(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->course_record[i].course_desc);
    }
    printf("\n");
    pos = search_pos_of_course_desc(key, ptr, n);
    if (pos < n && atoi(key) == atoi(ptr->course_record[pos].course_desc)){
      printf("Degree: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int search_pos_of_course_desc(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && atoi(newkey) > atoi(key_arr->course_record[pos].course_desc) ){
    pos++;
  }
  return pos;
  
}


/*-------------------------------------------SEARCH NO_OF_UNITS-------------------------------------------*/

void search_by_no_of_units(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->course_record[i].no_of_units);
    }
    printf("\n");
    pos = search_pos_of_no_of_units(key, ptr, n);
    if (pos < n && key == ptr->course_record[pos].no_of_units){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
}/*End of search()*/


int search_pos_of_no_of_units(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->course_record[pos].no_of_units ){
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCH HAS_LAB-------------------------------------------*/

void search_by_has_lab(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->course_record[i].has_lab);
    }
    printf("\n");
    pos = search_pos_of_has_lab(key, ptr, n);
    if (pos < n && key == ptr->course_record[pos].has_lab){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
}/*End of search()*/



int search_pos_of_has_lab(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->course_record[pos].has_lab ){
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCH SEM_OFFERED-------------------------------------------*/
void search_sem_offered(enum Semester semoffered){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->course_record[i].semoffered);
    }
    printf("\n");
    pos = search_pos_of_sem_offered(semoffered, ptr, n);
    if (pos < n && semoffered == ptr->course_record[pos].semoffered){
      printf("Sem Offered: %d found in position %d of last dispalyed node\n",semoffered,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",semoffered);
}/*End of search()*/

int search_pos_of_sem_offered(enum Semester semoffered, struct node *key_arr, int n){
  int pos=0;
  enum Semester newkey;
  newkey = semoffered;
  while (pos < n && newkey > key_arr->course_record[pos].semoffered ){
    pos++;
  }
  return pos;
  
}
/*----------------------I added the ff-------------------*/
info getCourseInfo(){
	info e;
	int i;
	
	//get info from user
    printf("Enter course no: ");
    scanf("%s", e.course_no);
	printf("Enter course_title: ");
    scanf("%s", e.course_title);
    printf("Enter course_desc: ");
    scanf("%s", e.course_desc);
    printf("With Laboratory? [1] Yes [0] No\t  ");
    scanf("%d", &e.has_lab);
    printf("Enter no. of units: ");
	scanf("%d", &e.no_of_units);
	printf("Enter sem offered: [1] 1st [2] 2nd [3] Sum\t ");
	scanf("%d", &i);
	
	switch(i){
		case 1: e.semoffered = First; break;
		case 2: e.semoffered = Second; break;
		case 3: e.semoffered = Sum; break;
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
                      printf("| %s %s %s %d %d ", ptr->course_record[i].course_no,ptr->course_record[i].course_title,ptr->course_record[i].course_desc,ptr->course_record[i].has_lab,ptr->course_record[i].no_of_units);
                  		
                  	switch(ptr->course_record[i].semoffered){	
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


void insertCourseRec(info y){
       struct node *newnode;
       char upKey[11];
       enum KeyStatus value;
       char key[11];
       
       strcpy(key,y.course_no);
       value = ins(root, key, upKey, &newnode, y);
       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;
          strcpy(root->course_record[0].course_no,upKey);
          strcpy(root->course_record[0].course_title,y.course_title);
          strcpy(root->course_record[0].course_desc,y.course_desc);
          root->course_record[0].has_lab = y.has_lab;
          root->course_record[0].no_of_units = y.no_of_units;
          root->course_record[0].semoffered = y.semoffered;
          
          root->p[0] = uproot;
          root->p[1] = newnode;
         
       }else{
       }
     
}


enum KeyStatus ins(struct node *ptr, char *key, char *upKey,struct node **newnode,info course_record){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    char lastKey[11];
    char lastctitle[50];
    char lastcdesc[50];
    enum Semester lastsemoffered;
    int lasthaslab;
    int lastnoofunits;
    enum KeyStatus value;
    char newKey[11];

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			//upKey = key;
			strcpy(upKey,key);
			return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos(key, ptr, n, course_record); //course_record = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
		
		if (pos < n && strcmp(key, ptr->course_record[pos].course_no)==0  ){
			return Duplicate;
		}
		value = ins(ptr->p[pos], key, newKey, &newPtr, course_record);

		if (value != InsertIt){
			return value;
		}

//If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
			pos = searchPos(newKey, ptr, n, course_record);
			//Shifting the key and pointer right for inserting the new key/
			for (i=n; i>pos; i--){
				ptr->course_record[i] = ptr->course_record[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			//Key is inserted at exact location
			strcpy(ptr->course_record[pos].course_no,newKey);
			strcpy(ptr->course_record[pos].course_title,course_record.course_title);
      		strcpy(ptr->course_record[pos].course_desc,course_record.course_desc);
			ptr->course_record[pos].has_lab,course_record.has_lab;
            ptr->course_record[pos].no_of_units = course_record.no_of_units;
            ptr->course_record[pos].semoffered = course_record.semoffered;
			ptr->p[pos+1] = newPtr;
			++ptr->n; //incrementing the number of keys in node

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			strcpy(lastKey, newKey);  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
			strcpy(lastctitle, course_record.course_title);
            strcpy(lastcdesc, course_record.course_desc);
            lasthaslab = course_record.has_lab;
            lastnoofunits=course_record.no_of_units;
            lastsemoffered=course_record.semoffered;
            
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			strcpy(lastKey,ptr->course_record[M-2].course_no);
			//added the following -mgaga
            strcpy(lastctitle,ptr->course_record[M-2].course_title);
            strcpy(lastcdesc,ptr->course_record[M-2].course_desc);
            lasthaslab = ptr->course_record[M-2].has_lab;
            lastnoofunits=ptr->course_record[M-2].no_of_units;
            lastsemoffered=ptr->course_record[M-2].semoffered;
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->course_record[i] = ptr->course_record[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			strcpy(ptr->course_record[pos].course_no,newKey);
			//added the ff -mgaga
			strcpy(ptr->course_record[pos].course_title,course_record.course_title);
			strcpy(ptr->course_record[pos].course_desc,course_record.course_desc);
			ptr->course_record[pos].has_lab	 = course_record.has_lab;
            ptr->course_record[pos].no_of_units = course_record.no_of_units;
            ptr->course_record[pos].semoffered = course_record.semoffered;
            
			ptr->p[pos+1] = newPtr;
		}
		splitPos = (M - 1)/2;
	//	(*upKey) = ptr->keys[splitPos];
	 //   printf("SPLITPOS: %d",splitPos);
        strcpy(upKey,ptr->course_record[splitPos].course_no);
	  //  printf("UPKEY: %s, %s",upKey, ptr->course_record[splitPos].course_no);		
	    
        (*newnode)=malloc(sizeof(struct node));//Right node after split
		ptr->n = splitPos; //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
			if(i < (*newnode)->n-1){
				strcpy((*newnode)->course_record[i].course_no, ptr->course_record[i + splitPos + 1].course_no);
				strcpy((*newnode)->course_record[i].course_title, ptr->course_record[i + splitPos + 1].course_title);
				strcpy((*newnode)->course_record[i].course_desc, ptr->course_record[i + splitPos + 1].course_desc);
				(*newnode)->course_record[i].has_lab = ptr->course_record[i + splitPos + 1].has_lab;
				(*newnode)->course_record[i].no_of_units = ptr->course_record[i + splitPos + 1].no_of_units;
				(*newnode)->course_record[i].semoffered = ptr->course_record[i + splitPos + 1].semoffered;
			}else{
				strcpy( (*newnode)->course_record[i].course_no,lastKey );
				strcpy( (*newnode)->course_record[i].course_title,lastctitle );
				strcpy( (*newnode)->course_record[i].course_desc,lastcdesc );
				
				(*newnode)->course_record[i].no_of_units = lastnoofunits;
				(*newnode)->course_record[i].has_lab = lasthaslab;
				(*newnode)->course_record[i].semoffered = lastsemoffered;
			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of ins()

int searchPos(char *key, struct node *key_arr, int n, info course_record){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->course_record[pos].course_no) ){
		pos++;
	}
	return pos;
	
}//End of searchPos()

int searchPosOfCno(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->course_record[pos].course_no) ){
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
			printf(" %s",ptr->course_record[i].course_no);
		}
		printf("\n");
		pos = searchPosOfCno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->course_record[pos].course_no)){
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

	pos = searchPosOfCno(key, knode, n);	

	if(p[0] == NULL){
                if (pos == n || (atoi(key) < atoi(knode->course_record[pos].course_no)))
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->course_record[i-1] = knode->course_record[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && (atoi(key) == atoi(knode->course_record[pos].course_no)) ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->course_record[pos] = qp->course_record[nkey-1];
                //qp->keys[nkey - 1] = key;
		strcpy(qp->course_record[nkey-1].course_no,key);
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
                        rptr->course_record[i] = rptr->course_record[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->course_record[0] = knode->course_record[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->course_record[pivot] = lptr->course_record[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
                lptr->course_record[lptr->n] = knode->course_record[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                knode->course_record[pivot] = rptr->course_record[0];
                lptr->n++;
                rptr->n--;

                for(i=0; i < rptr->n; i++){
                        rptr->course_record[i] = rptr->course_record[i+1];
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
        lptr->course_record[lptr->n] = knode->course_record[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
       
        for(i=0; i < rptr->n; i++){
                lptr->course_record[lptr->n + 1 + i] = rptr->course_record[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
        }

        lptr->n = lptr->n + rptr->n +1;
        free(rptr); /*Remove right node*/

        for (i=pos+1; i < n; i++){
                knode->course_record[i-1] = knode->course_record[i];
                p[i] = p[i+1];
        }

        return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);


}//End of del()


