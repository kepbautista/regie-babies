#define M 5

    
enum Semester {First, Second, Sum};

typedef struct courseRec{
	char course_no[11];
	char course_title[50];	
	char course_desc[50];	
  int has_lab;
  int no_of_units;
  enum Semester sem_offered;	
	
}info;

struct node{

       info courseRec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};


struct node *root=NULL;  
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };

//INSERT FUNCTIONS
info get_course_info();
void insert_c_record(info y);
enum KeyStatus ins_c_course(struct node *ptr, char *key, char *upKey, char *upCourseTitle, char *upCourseDesc, int *upHasLab, int *upNoOfUnits, enum Semester *upSemOffered, struct node **newnode,info courseRec);
int searchPos_course(char *x,struct node *key_arr, int n, info courseRec);

//DELETE FUNCTIONS
void delete_c_record(struct node **root,char *x);
enum KeyStatus delete_c_course(struct node **root,struct node *ptr, char *key);


//DISPLAY FUNCTIONS
void display_c_record(struct node *root,int);
void display_sc_asc(struct node *ptr); 

//SEARCH FUNCTIONS
void search_c_data(int x);
void search_c_courseno(char *key);
void search_c_coursetitle(char *key);
void search_c_coursedesc(char *key);
void search_c_haslab(int key);
void search_c_noofunits(int key);
void search_c_semoffered(enum Semester sem_offered);
int searchPos_c_cno(char *x,struct node *key_arr, int n);
int searchPos_c_ctitle(char *key, struct node *key_arr, int n);
int searchPos_c_cdesc(char *key, struct node *key_arr, int n);
int searchPos_c_noofunits(int key, struct node *key_arr, int n);
int searchPos_c_haslab(int key, struct node *key_arr, int n);
int searchPos_c_semoffered(enum Semester sem_offered, struct node *key_arr, int n);

//FILES FUNCTIONS
void save_c_data_file(struct node *root);
void save_c_data(FILE *fp,struct node *ptr);
void load_c_data_file(struct node **root);





/*----------------------I added the ff-------------------*/
info get_course_info(){
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
		case 1: e.sem_offered = First; break;
		case 2: e.sem_offered = Second; break;
		case 3: e.sem_offered = Sum; break;
		default: printf("Wrong choice.\n");
	}
	
	return e;
}

void insert_c_record(info y){
       struct node *newnode;
       char upKey[11];
       char upCourseTitle[50];
       char upCourseDesc[15];
       int upHasLab;
       int upNoOfUnits;
       enum Semester upSemOffered;

       enum KeyStatus value;
       char key[11];

       strcpy(key,y.course_no);
       value = ins_c_course(root, key, upKey, upCourseTitle, upCourseDesc, &upHasLab, &upNoOfUnits, &upSemOffered, &newnode, y);
       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;

          strcpy(root->courseRec[0].course_no,upKey);
          strcpy(root->courseRec[0].course_title,upCourseTitle);
          strcpy(root->courseRec[0].course_desc,upCourseDesc);
          root->courseRec[0].has_lab = upHasLab;
          root->courseRec[0].no_of_units = upNoOfUnits;
          root->courseRec[0].sem_offered = upSemOffered;
          
          root->p[0] = uproot;
          root->p[1] = newnode;
         
       }else{
       }
     
}


enum KeyStatus ins_c_course(struct node *ptr, char *key, char *upKey, char *upCourseTitle, char *upCourseDesc, int *upHasLab, int *upNoOfUnits, enum Semester *upSemOffered, struct node **newnode,info courseRec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    char lastKey[11];
    char lastctitle[50];
    char lastcdesc[50];
    enum Semester lastsem_offered;
    int lasthaslab;
    int lastnoofunits;
    enum KeyStatus value;
    char newKey[11];

    //last new keys!
    char nctitle[50];
    char ncdesc[50];
    enum Semester nsemoffered;
    int nhaslab;
    int nnoofunits;

    if (ptr == NULL){    

      *newnode = NULL;
        //upKey = key;
        strcpy(upKey,key);
        strcpy(upCourseTitle,courseRec.course_title);
        strcpy(upCourseDesc,courseRec.course_desc);
        *upNoOfUnits=courseRec.no_of_units;
        *upHasLab=courseRec.has_lab;
        *upSemOffered=courseRec.sem_offered;
        return InsertIt;
    }

    n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
    pos = searchPos_course(key, ptr, n, courseRec); //courseRec = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
    
    if (pos < n && strcmp(key, ptr->courseRec[pos].course_no)==0  ){
      return Duplicate;

    }

    //HEY!
    value = ins_c_course(ptr->p[pos], key, newKey,nctitle,ncdesc,&nhaslab,&nnoofunits, &nsemoffered, &newPtr, courseRec); 

    if (value != InsertIt){
      return value;
    }

//If keys in node is less than M-1 where M is order of B tree
    if (n < M - 1){
      pos = searchPos_course(newKey, ptr, n, courseRec);
      //Shifting the key and pointer right for inserting the new key/
      for (i=n; i>pos; i--){
        ptr->courseRec[i] = ptr->courseRec[i-1];
        ptr->p[i+1] = ptr->p[i];
      }
        strcpy(ptr->courseRec[pos].course_no,newKey);
        strcpy(ptr->courseRec[pos].course_title,nctitle);
        strcpy(ptr->courseRec[pos].course_desc,ncdesc);
        ptr->courseRec[pos].has_lab = nhaslab;
        ptr->courseRec[pos].no_of_units= nnoofunits;
        ptr->courseRec[pos].sem_offered = nsemoffered;

        ptr->p[pos+1] = newPtr;
        ++ptr->n;


      return Success;
    }//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
    if (pos == M-1){
      strcpy(lastKey, newKey);  //ilipat lahat ng laman ng struct dito? +name, deg etc?
      //added the ff - mgaga
      strcpy(lastctitle, courseRec.course_title);
            strcpy(lastcdesc, courseRec.course_desc);
            lasthaslab = courseRec.has_lab;
            lastnoofunits=courseRec.no_of_units;
            lastsem_offered=courseRec.sem_offered;
            
            lastPtr = newPtr;
    
    }else{
    //If keys in node are maximum and position of node to be inserted is not last
      strcpy(lastKey,ptr->courseRec[M-2].course_no);
      //added the following -mgaga
            strcpy(lastctitle,ptr->courseRec[M-2].course_title);
            strcpy(lastcdesc,ptr->courseRec[M-2].course_desc);
            lasthaslab = ptr->courseRec[M-2].has_lab;
            lastnoofunits=ptr->courseRec[M-2].no_of_units;
            lastsem_offered=ptr->courseRec[M-2].sem_offered;
            lastPtr = ptr->p[M-1];
            
      for (i=M-2; i>pos; i--){
        ptr->courseRec[i] = ptr->courseRec[i-1];
        ptr->p[i+1] = ptr->p[i];
      }
      strcpy(ptr->courseRec[pos].course_no,newKey);
      //added the ff -mgaga
      strcpy(ptr->courseRec[pos].course_title,courseRec.course_title);
      strcpy(ptr->courseRec[pos].course_desc,courseRec.course_desc);
      ptr->courseRec[pos].has_lab  = courseRec.has_lab;
      ptr->courseRec[pos].no_of_units = courseRec.no_of_units;
      ptr->courseRec[pos].sem_offered = courseRec.sem_offered;
            
      ptr->p[pos+1] = newPtr;
    }
    splitPos = (M - 1)/2;
    strcpy(upKey,ptr->courseRec[splitPos].course_no);
    strcpy(upCourseTitle,ptr->courseRec[splitPos].course_title);
    strcpy(upCourseDesc,ptr->courseRec[splitPos].course_title);
    *upHasLab = ptr->courseRec[splitPos].has_lab;
    *upNoOfUnits = ptr->courseRec[splitPos].no_of_units;
    *upSemOffered = ptr->courseRec[splitPos].sem_offered;
      
    (*newnode)=malloc(sizeof(struct node));//Right node after split
    ptr->n = splitPos; //No. of keys for left splitted node
    (*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
    
    for (i=0; i < (*newnode)->n; i++){
      (*newnode)->p[i] = ptr->p[i + splitPos + 1];
      if(i < (*newnode)->n-1){
        strcpy((*newnode)->courseRec[i].course_no, ptr->courseRec[i + splitPos + 1].course_no);
        strcpy((*newnode)->courseRec[i].course_title, ptr->courseRec[i + splitPos + 1].course_title);
        strcpy((*newnode)->courseRec[i].course_desc, ptr->courseRec[i + splitPos + 1].course_desc);
        (*newnode)->courseRec[i].has_lab = ptr->courseRec[i + splitPos + 1].has_lab;
        (*newnode)->courseRec[i].no_of_units = ptr->courseRec[i + splitPos + 1].no_of_units;
        (*newnode)->courseRec[i].sem_offered = ptr->courseRec[i + splitPos + 1].sem_offered;
      }else{
        strcpy( (*newnode)->courseRec[i].course_no,lastKey );
        strcpy( (*newnode)->courseRec[i].course_title,lastctitle );
        strcpy( (*newnode)->courseRec[i].course_desc,lastcdesc );
        
        (*newnode)->courseRec[i].no_of_units = lastnoofunits;
        (*newnode)->courseRec[i].has_lab = lasthaslab;
        (*newnode)->courseRec[i].sem_offered = lastsem_offered;
      }
    }
    (*newnode)->p[(*newnode)->n] = lastPtr;
    
    return InsertIt;
    
}//End of ins_c_course()

int searchPos_course(char *key, struct node *key_arr, int n, info courseRec){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && strcmp(newkey, key_arr->courseRec[pos].course_no)>0 ){
    pos++;
  }
  return pos;
  
}//End of searchPos_course()




void search_c_data(int x){
     char course_num[11];
     char course_title[50];
     char course_desc[11];
     int has_lab;
     int no_of_units;
     int so;
     enum Semester sem_offered;

     switch(x){
        case 1:
                         printf("Enter course number: \n"); 
                         scanf("%s",course_num);
                         search_c_courseno(course_num);
                         break;
                                 
        case 2:          printf("Enter course title: \n"); 
                         scanf("%s",course_title);
                         search_c_coursetitle(course_title); 
                         break;
        case 3:
                         printf("Enter course description: \n"); 
                         scanf("%s",course_desc);
                         search_c_coursedesc(course_desc); 
                         break;
        case 4:
                         printf("Enter no of units: \n"); 
                         scanf("%d",&no_of_units);
                         search_c_noofunits(no_of_units); 
                         break;

        case 5:          printf("Course with lab? \n[1] Yes [2] No \t"); 
                         scanf("%d",&has_lab);
                         search_c_haslab(has_lab);
                         break;

        case 6:         printf("Sem Offered? \n[0] 1st [1] 2nd [2] Sum\t"); 
                         scanf("%d",&so);
                         if(so==0){sem_offered = First;}
                          else if(so==1){sem_offered = Second;}
                            else if(so==2){sem_offered = Sum;}
                         search_c_semoffered(sem_offered);
                         break;
        default: printf("INVALID");

      }
     
}

/*-------------------------------------------SEARCH COURSE_NO-------------------------------------------*/

void search_c_courseno(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->courseRec[i].course_no);
    }
    printf("\n");
    pos = searchPos_c_cno(key, ptr, n);
    if (pos < n && strcmp(key, ptr->courseRec[pos].course_no)==0){
      printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int searchPos_c_cno(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && strcmp(newkey, key_arr->courseRec[pos].course_no) > 0){
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCH COURSE_TITLE-------------------------------------------*/

void search_c_coursetitle(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->courseRec[i].course_title);
    }
    printf("\n");
    pos = searchPos_c_ctitle(key, ptr, n);
    if (pos < n && strcmp(key, ptr->courseRec[pos].course_title)==0){
      printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int searchPos_c_ctitle(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && strcmp(newkey, key_arr->courseRec[pos].course_title)>0 ){
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCH COURSE_DESC-------------------------------------------*/

void search_c_coursedesc(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->courseRec[i].course_desc);
    }
    printf("\n");
    pos = searchPos_c_cdesc(key, ptr, n);
    if (pos < n && strcmp(key, ptr->courseRec[pos].course_desc)==0){
      printf("Degree: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int searchPos_c_cdesc(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && strcmp(newkey, key_arr->courseRec[pos].course_desc)>0){
    pos++;
  }
  return pos;
  
}


/*-------------------------------------------SEARCH NO_OF_UNITS-------------------------------------------*/

void search_c_noofunits(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->courseRec[i].no_of_units);
    }
    printf("\n");
    pos = searchPos_c_noofunits(key, ptr, n);
    if (pos < n && key == ptr->courseRec[pos].no_of_units){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
}/*End of search()*/


int searchPos_c_noofunits(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->courseRec[pos].no_of_units ){
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCH HAS_LAB-------------------------------------------*/

void search_c_haslab(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->courseRec[i].has_lab);
    }
    printf("\n");
    pos = searchPos_c_haslab(key, ptr, n);
    if (pos < n && key == ptr->courseRec[pos].has_lab){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
}/*End of search()*/



int searchPos_c_haslab(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->courseRec[pos].has_lab ){
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCH SEM_OFFERED-------------------------------------------*/
void search_c_semoffered(enum Semester sem_offered){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->courseRec[i].sem_offered);
    }
    printf("\n");
    pos = searchPos_c_semoffered(sem_offered, ptr, n);
    if (pos < n && sem_offered == ptr->courseRec[pos].sem_offered){
      printf("Sem Offered: %d found in position %d of last dispalyed node\n",sem_offered,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",sem_offered);
}/*End of search()*/

int searchPos_c_semoffered(enum Semester sem_offered, struct node *key_arr, int n){
  int pos=0;
  enum Semester newkey;
  newkey = sem_offered;
  while (pos < n && newkey > key_arr->courseRec[pos].sem_offered ){
    pos++;
  }
  return pos;
  
}

void display(struct node *ptr, int blanks){
    if(ptr){
		int i,j;
		for(i=1;i<=blanks;i++){
			printf(" ");
		}
		for (i=0; i < ptr->n; i++){
                      printf("| %s %s %s %d %d ", ptr->courseRec[i].course_no,ptr->courseRec[i].course_title,ptr->courseRec[i].course_desc,ptr->courseRec[i].has_lab,ptr->courseRec[i].no_of_units);
                  		
                  	switch(ptr->courseRec[i].sem_offered){	
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


void display_c_asc(struct node *ptr) {
        int i;
        if (ptr) {
                for (i = 0; i < ptr->n; i++) {
                   display_c_asc(ptr->p[i]);
                    printf("| %s %s %s %d %d ", ptr->courseRec[i].course_no,ptr->courseRec[i].course_title,ptr->courseRec[i].course_desc,ptr->courseRec[i].has_lab,ptr->courseRec[i].no_of_units);
                      
                    switch(ptr->courseRec[i].sem_offered){  
                      case 0: printf("1st |\n"); break;
                      case 1: printf("2nd |\n"); break;
                      case 2: printf("3rd |\n"); break;                
                  }
                }
                  display_c_asc(ptr->p[i]);
        }
  }


void search(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->courseRec[i].course_no);
		}
		printf("\n");
		pos = searchPos_c_cno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->courseRec[pos].course_no)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/


void delete_c_record(struct node **root, char *key){
	struct node *uproot;
	enum KeyStatus value;
	
	value = delete_c_course(root,*root,key);
	
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

enum KeyStatus delete_c_course(struct node **root,struct node *ptr, char *key){
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

	pos = searchPos_c_cno(key, knode, n);	

	if(p[0] == NULL){
                if (pos == n || (atoi(key) < atoi(knode->courseRec[pos].course_no)))
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->courseRec[i-1] = knode->courseRec[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && (atoi(key) == atoi(knode->courseRec[pos].course_no)) ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->courseRec[pos] = qp->courseRec[nkey-1];
                //qp->keys[nkey - 1] = key;
		strcpy(qp->courseRec[nkey-1].course_no,key);
        }//End of if

        value = delete_c_course(root, p[pos], key);
        if (value != LessKeys) return value;

        if (pos > 0 && p[pos-1]->n > min){
                pivot = pos - 1; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pos];
                /*Assigns values for right node*/
                rptr->p[rptr->n + 1] = rptr->p[rptr->n];
                for (i=rptr->n; i>0; i--){
                        rptr->courseRec[i] = rptr->courseRec[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->courseRec[0] = knode->courseRec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->courseRec[pivot] = lptr->courseRec[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
                lptr->courseRec[lptr->n] = knode->courseRec[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                knode->courseRec[pivot] = rptr->courseRec[0];
                lptr->n++;
                rptr->n--;

                for(i=0; i < rptr->n; i++){
                        rptr->courseRec[i] = rptr->courseRec[i+1];
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
        lptr->courseRec[lptr->n] = knode->courseRec[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
       
        for(i=0; i < rptr->n; i++){
                lptr->courseRec[lptr->n + 1 + i] = rptr->courseRec[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
        }

        lptr->n = lptr->n + rptr->n +1;
        free(rptr); /*Remove right node*/

        for (i=pos+1; i < n; i++){
                knode->courseRec[i-1] = knode->courseRec[i];
                p[i] = p[i+1];
        }

        return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);


}//End of delete_c_course()


/*START OF FILES FUNCTIONS*/
void save_c_data_file(struct node *root){
  FILE* fp=fopen("course.txt","w");
  save_c_data(fp,root);
  fclose(fp);
  printf("Student information are saved!\n");
}
void save_c_data(FILE *fp,struct node *ptr){
  int i, j;
    if(ptr){
    for (i=0; i < ptr->n; i++){
              fprintf(fp,"%s|%s|%s|%d|%d|%d\n",ptr->courseRec[i].course_no, ptr->courseRec[i].course_title,ptr->courseRec[i].course_desc,ptr->courseRec[i].has_lab,ptr->courseRec[i].no_of_units,ptr->courseRec[i].sem_offered);

    }
    printf("\n");
    for (i=0; i <= ptr->n; i++){
      save_c_data(fp, ptr->p[i]);
    }
        
  }
}

void load_c_data_file(struct node **root){
  char c, line[200], *token, tok[20];
  FILE* fp=fopen("course.txt","r");//reads the file
  info x;
  int i=0,j,a=0;//counter
  
  //if file does not exist or has no contents
  if(fp==NULL || ((c=getc(fp))==EOF))
    printf("\nNo STUDENT RECORD yet!\n");
  //read each line of the text file
  else{
    ungetc(c,fp);
    while(!feof(fp)){
      if(fgets(line,200,fp)!=NULL){
      
        //get first token
        token = strtok(line,"|");
        
        while(token!=NULL){
          switch(i){
            case 0: strcpy(x.course_no,token);
                break;
            case 1: strcpy(x.course_title,token);  //strcpy(tok,token)
                break;
            case 2: strcpy(x.course_desc,token);  //strcpy(tok,token)
                break;
            case 3: x.has_lab=atoi(token);  //strcy(tok,token)
                break;
            case 4: x.no_of_units=atoi(token);  //strcy(tok,token)
                break;
            case 5: x.sem_offered=atoi(token);
                    break;
          }
          token=strtok(NULL,"|");
          i++;

        }
        a++;
        //printf("%s %s %s %s %s %d\n", x.stdno, x.sname, x.std_bday, x.degree,  x.std_major, x.units);
        insert_c_record(x);//place info inside the Btree
      }
      i=0;
    }
    printf("Student information are loaded! %d\n",a);
  }
  fclose(fp);
}
