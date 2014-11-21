#define M 5

enum Semester {First, Second, Sum};

typedef struct courseoffering_record{
	/*Place necessary fields here*/
	int id;
	enum Semester semester;	
	char acad_year[50];
	char course_no[11];
	char section[50];
	char time[50];	
    	int max_stud;
    		
}info;

struct node{

       info course_offering_rec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};


struct node *root=NULL;
 

info getCourseOfferingInfo(int *id);
void insertCourseOfferingRec(info y);
enum KeyStatus ins(struct node *r, int *x, int *y, struct node** u, info course_offering_rec);
int searchPos(int *x,struct node *key_arr, int n, info course_offering_rec);
void display(struct node *root,int);
void DelNode(struct node **root,int x);
enum KeyStatus del(struct node **root,struct node *ptr, int key);
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };

void search_by_course_id(int key);
void search_by_max_stud(int key);
void search_semester(enum Semester semester);
void search_by_acad_year(char *key);
void search_by_time(char *key);
void search_by_course_no(char *key);
void search_by_section(char *key);


void search_courseoffering_data(x){
	int id;
    char course_no[11];
    char acad_year[50];
    char section[11];
    int max_stud;
    char time[50];
    int so;
    enum Semester semester;

     switch(x){
        case 1:
                         printf("Enter course id: \n"); 
                         scanf("%d",&id);
                         search_by_course_id(id);
                         break;
                                 
        case 2: 
                      	printf("Enter course num: \n"); 
                         scanf("%s",course_no);
                         search_by_course_no(course_no); 
                         break;
        case 3:
                       printf("Enter acad year: \n"); 
                         scanf("%s",acad_year);
                         search_by_acad_year(acad_year); 
                         break;
        case 4:
                       printf("Enter Max no. of Students: \n"); 
                         scanf("%d",&max_stud);
                         search_by_max_stud(max_stud); 
                         break;

        case 5:
                       printf("Enter time: \n"); 
                         scanf("%s",time);
                         search_by_time(time); 
                         break;

        case 6:
                       printf("Enter section: \n"); 
                         scanf("%s",section);
                         search_by_section(section); 
                         break;


        case 7:         printf("Sem Offered? \n[0] 1st [1] 2nd [2] Sum\t"); 
                         scanf("%d",&so);
                         if(so==0){semester = First;}
                          else if(so==1){semester = Second;}
                            else if(so==2){semester = Sum;}
                         search_semester(semester);
                         break;
        default: printf("INVALID");

      }
     
}

/*-------------------------------------------SEARCHES-------------------------------------------*/

void search_by_course_no(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->course_offering_rec[i].course_no);
    }
    printf("\n");
    pos = search_pos_of_course_no(key, ptr, n);
    if (pos < n && atoi(key) == atoi(ptr->course_offering_rec[pos].course_no)){
      printf("Course No: %s found in position %d of last dispalyed node\n",key,i);
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
  while (pos < n && atoi(newkey) > atoi(key_arr->course_offering_rec[pos].course_no) ){
    pos++;
  }
  return pos;
  
}

void search_by_acad_year(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->course_offering_rec[i].acad_year);
    }
    printf("\n");
    pos = search_pos_of_acad_year(key, ptr, n);
    if (pos < n && atoi(key) == atoi(ptr->course_offering_rec[pos].acad_year)){
      printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int search_pos_of_acad_year(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && atoi(newkey) > atoi(key_arr->course_offering_rec[pos].acad_year) ){
    pos++;
  }
  return pos;
  
}

void search_by_section(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->course_offering_rec[i].section);
    }
    printf("\n");
    pos = search_pos_of_section (key, ptr, n);
    if (pos < n && atoi(key) == atoi(ptr->course_offering_rec[pos].section)){
      printf("Degree: %s found in position %d of last displayed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int search_pos_of_section (char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && atoi(newkey) > atoi(key_arr->course_offering_rec[pos].section) ){
    pos++;
  }
  return pos;
  
}

void search_by_time(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->course_offering_rec[i].time);
    }
    printf("\n");
    pos = search_pos_of_time (key, ptr, n);
    if (pos < n && atoi(key) == atoi(ptr->course_offering_rec[pos].time)){
      printf("Degree: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
}/*End of search()*/

int search_pos_of_time(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && atoi(newkey) > atoi(key_arr->course_offering_rec[pos].time) ){
    pos++;
  }
  return pos;
  
}



void search_by_course_id(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->course_offering_rec[i].id);
    }
    printf("\n");
    pos = search_pos_of_course_id(key, ptr, n);
    if (pos < n && key == ptr->course_offering_rec[pos].id){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
}/*End of search()*/

int search_pos_of_course_id(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->course_offering_rec[pos].id ){
    pos++;
  }
  return pos;
  
}

void search_by_max_stud(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->course_offering_rec[i].max_stud);
    }
    printf("\n");
    pos = search_pos_of_max_stud(key, ptr, n);
    if (pos < n && key == ptr->course_offering_rec[pos].max_stud){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
}/*End of search()*/

int search_pos_of_max_stud(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->course_offering_rec[pos].max_stud ){
    pos++;
  }
  return pos;
  
}


void search_semester(enum Semester semester){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->course_offering_rec[i].semester);
    }
    printf("\n");
    pos = search_pos_of_sem_offered(semester, ptr, n);
    if (pos < n && semester == ptr->course_offering_rec[pos].semester){
      printf("Sem Offered: %d found in position %d of last dispalyed node\n",semester,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",semester);
}/*End of search()*/

int search_pos_of_sem_offered(enum Semester semester, struct node *key_arr, int n){
  int pos=0;
  enum Semester newkey;
  newkey = semester;
  while (pos < n && newkey > key_arr->course_offering_rec[pos].semester ){
    pos++;
  }
  return pos;
  
}
/*----------------------I added the ff-------------------*/
info getCourseOfferingInfo(int *cid){
	info e;
	int i;	
	e.id = *cid;
	(*cid)++;
	//get info from user
    printf("Enter course no: ");
    scanf("%s", e.course_no);
	printf("Enter acad_year: ");
    scanf("%s", e.acad_year);
    printf("Enter section: ");
    scanf("%s", e.section);
    printf("Enter time: ");
    scanf("%s", e.time);
    printf("Enter max no. of students: ");
	scanf("%d",&e.max_stud);

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
                      printf("| %d %s %s %s %s %d ", ptr->course_offering_rec[i].id, ptr->course_offering_rec[i].course_no,ptr->course_offering_rec[i].acad_year,ptr->course_offering_rec[i].section,ptr->course_offering_rec[i].time,ptr->course_offering_rec[i].max_stud);
                  		
                  	switch(ptr->course_offering_rec[i].semester){	
                  		case 0: printf("1st |"); break;
                  		case 1: printf("2nd |"); break;
                  		case 2: printf("Sum |"); break;
                  		
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
          root->course_offering_rec[0].id = upKey;
          strcpy(root->course_offering_rec[0].course_no,y.course_no);
          strcpy(root->course_offering_rec[0].acad_year,y.acad_year);
          strcpy(root->course_offering_rec[0].section,y.section);
          strcpy(root->course_offering_rec[0].time,y.time);
          root->course_offering_rec[0].max_stud = y.max_stud;
          root->course_offering_rec[0].semester = y.semester;
          
          root->p[0] = uproot;
          root->p[1] = newnode;
         
       }else{
       }
     
}


enum KeyStatus ins(struct node *ptr, int *key, int *upKey,struct node **newnode,info course_offering_rec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    int lastKey;
    char lastacadyear[50];
    char lastsection[50];
    char lasttime[50];
    char lastcno[11];
    
    enum Semester lastsem;
    int lastmaxstud;
    enum KeyStatus value;
    int newKey;

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			*upKey = *key;
			return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos(key, ptr, n, course_offering_rec); //course_offering_rec = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
		
		if (pos < n && *key == ptr->course_offering_rec[pos].id ){
			return Duplicate;
		}
		value = ins(ptr->p[pos], key, &newKey, &newPtr, course_offering_rec);

		if (value != InsertIt){
			return value;
		}

//If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
			pos = searchPos(&newKey, ptr, n, course_offering_rec);
			//Shifting the key and pointer right for inserting the new key/
			for (i=n; i>pos; i--){
				ptr->course_offering_rec[i] = ptr->course_offering_rec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			//Key is inserted at exact location
			
			ptr->course_offering_rec[pos].id = newKey;
			strcpy(ptr->course_offering_rec[pos].course_no,course_offering_rec.course_no);
			strcpy(ptr->course_offering_rec[pos].acad_year,course_offering_rec.acad_year);
      		strcpy(ptr->course_offering_rec[pos].section,course_offering_rec.section);
      		strcpy(ptr->course_offering_rec[pos].time,course_offering_rec.time);
			ptr->course_offering_rec[pos].max_stud = course_offering_rec.max_stud;
            ptr->course_offering_rec[pos].semester = course_offering_rec.semester;
			ptr->p[pos+1] = newPtr;
			++ptr->n; //incrementing the number of keys in node

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			lastKey = newKey;  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
			strcpy(lastcno, course_offering_rec.course_no);
            strcpy(lasttime, course_offering_rec.time);
            strcpy(lastacadyear, course_offering_rec.acad_year);
            strcpy(lastsection, course_offering_rec.section);
            lastmaxstud = course_offering_rec.max_stud;
            lastsem=course_offering_rec.semester;
            
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			lastKey = ptr->course_offering_rec[M-2].id;
			
			strcpy(lastcno, ptr->course_offering_rec[M-2].course_no);
            strcpy(lasttime, ptr->course_offering_rec[M-2].time);
            strcpy(lastacadyear, ptr->course_offering_rec[M-2].acad_year);
            strcpy(lastsection, ptr->course_offering_rec[M-2].section);
            lastmaxstud = ptr->course_offering_rec[M-2].max_stud;
            lastsem = ptr->course_offering_rec[M-2].semester;
            
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->course_offering_rec[i] = ptr->course_offering_rec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			ptr->course_offering_rec[pos].id = newKey;
			//added the ff -mgaga
			strcpy(ptr->course_offering_rec[pos].course_no,course_offering_rec.course_no);
			strcpy(ptr->course_offering_rec[pos].acad_year,course_offering_rec.acad_year);
			strcpy(ptr->course_offering_rec[pos].section, course_offering_rec.section);
			strcpy(ptr->course_offering_rec[pos].time, course_offering_rec.time);
            ptr->course_offering_rec[pos].max_stud = course_offering_rec.max_stud;
            ptr->course_offering_rec[pos].semester = course_offering_rec.semester;
            
			ptr->p[pos+1] = newPtr;
		}
		splitPos = (M - 1)/2;
        *upKey = ptr->course_offering_rec[splitPos].id;	
	    
        (*newnode)=malloc(sizeof(struct node));//Right node after split
		ptr->n = splitPos; //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
			if(i < (*newnode)->n-1){
				(*newnode)->course_offering_rec[i].id = ptr->course_offering_rec[i + splitPos + 1].id;
				strcpy((*newnode)->course_offering_rec[i].course_no, ptr->course_offering_rec[i + splitPos + 1].course_no);
				strcpy((*newnode)->course_offering_rec[i].acad_year, ptr->course_offering_rec[i + splitPos + 1].acad_year);
				strcpy((*newnode)->course_offering_rec[i].section, ptr->course_offering_rec[i + splitPos + 1].section);
				strcpy((*newnode)->course_offering_rec[i].time, ptr->course_offering_rec[i + splitPos + 1].time);
				(*newnode)->course_offering_rec[i].max_stud = ptr->course_offering_rec[i + splitPos + 1].max_stud;
				(*newnode)->course_offering_rec[i].semester = ptr->course_offering_rec[i + splitPos + 1].semester;
			}else{
				(*newnode)->course_offering_rec[i].id = lastKey;
				strcpy( (*newnode)->course_offering_rec[i].course_no,lastcno );
				strcpy( (*newnode)->course_offering_rec[i].acad_year,lastacadyear );
				strcpy( (*newnode)->course_offering_rec[i].section,lastsection );
				strcpy( (*newnode)->course_offering_rec[i].time,lasttime);
				
				(*newnode)->course_offering_rec[i].max_stud = lastmaxstud;
				(*newnode)->course_offering_rec[i].semester = lastsem;
			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of ins()



int searchPos(int *key, struct node *key_arr, int n, info course_offering_rec){
	int pos=0;
	int newkey;
	newkey = *key;
	while (pos < n && newkey > key_arr->course_offering_rec[pos].id ){
		pos++;
	}
	return pos;
	
}//End of searchPos()


int searchPosOfcno(int key, struct node *key_arr, int n){
	int pos=0;
	int newkey;
	newkey = key;
	while (pos < n && newkey > key_arr->course_offering_rec[pos].id ){
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
			printf(" %d",ptr->course_offering_rec[i].id);
		}
		printf("\n");
		pos = searchPosOfcno(key, ptr, n);
		if (pos < n && key == ptr->course_offering_rec[pos].id){
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
                if (pos == n || key < knode->course_offering_rec[pos].id)
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->course_offering_rec[i-1] = knode->course_offering_rec[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && key == knode->course_offering_rec[pos].id ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->course_offering_rec[pos] = qp->course_offering_rec[nkey-1];
                //qp->keys[nkey - 1] = key;
		qp->course_offering_rec[nkey-1].id = key;
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
                        rptr->course_offering_rec[i] = rptr->course_offering_rec[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->course_offering_rec[0] = knode->course_offering_rec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->course_offering_rec[pivot] = lptr->course_offering_rec[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
                lptr->course_offering_rec[lptr->n] = knode->course_offering_rec[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                knode->course_offering_rec[pivot] = rptr->course_offering_rec[0];
                lptr->n++;
                rptr->n--;

                for(i=0; i < rptr->n; i++){
                        rptr->course_offering_rec[i] = rptr->course_offering_rec[i+1];
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
        lptr->course_offering_rec[lptr->n] = knode->course_offering_rec[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
       
        for(i=0; i < rptr->n; i++){
                lptr->course_offering_rec[lptr->n + 1 + i] = rptr->course_offering_rec[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
        }

        lptr->n = lptr->n + rptr->n +1;
        free(rptr); /*Remove right node*/

        for (i=pos+1; i < n; i++){
                knode->course_offering_rec[i-1] = knode->course_offering_rec[i];
                p[i] = p[i+1];
        }

        return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);


}//End of del()
