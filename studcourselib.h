#define M 5

enum Semester {First, Second, Sum};

typedef struct studcourse_record{
	/*Place necessary fields here*/
	int id;
	char course_no[11];
	char acad_year[50];	
	char student_no[50];	
    enum Semester semester;	
	
}info;

struct node{

       info studentcourse_rec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};


struct node *root=NULL;


/*I ADDED THE FF---------------------------------------*/   

info getStudCourseInfo(int *id);
void insertCourseOfferingRec(info y);
enum KeyStatus ins(struct node *ptr, int *key, int *upKey, char *upCourseNo, char *upAcadYear, char *upStudentNo, enum Semester *semester, struct node** u, info studentcourse_rec);
int searchPos(int *x,struct node *key_arr, int n, info studentcourse_rec);
void display(struct node *root,int);
void DelNode(struct node **root,int x);
enum KeyStatus del(struct node **root,struct node *ptr, int key);
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };
/*----------------------I added the ff-------------------*/
void search_stdno(char *key);
void search_course_no(char *key);
void search_acad_year(char *key);
void search_id(int key);
void search_semester(enum Semester semester);

void saveStudentData(struct node *root);
void saveStData(FILE *fp,struct node *ptr);
void loadStudentData(struct node **root);

void search_studcourse_data(x){
     int id;
     char stdno[50];
     char acad_year[11];
     char course_no[11];
     enum Semester semoffered;
     int so;

     switch(x){
        case 1:
                         printf("Enter course number: \n"); 
                         scanf("%s",course_no);
                         search_course_no(course_no);
                         break;
                                 
        case 2: 
                       printf("Enter stdno: \n"); 
                         scanf("%s",stdno);
                         search_stdno(stdno); 
                         break;
        case 3:
                       printf("Enter acad_year: \n"); 
                         scanf("%s",acad_year);
                         search_acad_year(acad_year); 
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
      printf(" %d",ptr->studentcourse_rec[i].id);
    }
    printf("\n");
    pos = searchPosOfId(key, ptr, n);
    if (pos < n && key == ptr->studentcourse_rec[pos].id){
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
  while (pos < n && newkey > key_arr->studentcourse_rec[pos].id ){
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
			printf(" %s",ptr->studentcourse_rec[i].student_no);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studentcourse_rec[pos].student_no)){
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
	while (pos < n && atoi(newkey) > atoi(key_arr->studentcourse_rec[pos].student_no) ){
		pos++;
	}
	return pos;
	
}

void search_acad_year(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studentcourse_rec[i].acad_year);
		}
		printf("\n");
		pos = searchPosOfAcadYear(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studentcourse_rec[pos].acad_year)){
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
	while (pos < n && atoi(newkey) > atoi(key_arr->studentcourse_rec[pos].acad_year) ){
		pos++;
	}
	return pos;
	
}

void search_course_no(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studentcourse_rec[i].course_no);
		}
		printf("\n");
		pos = searchPosOfCno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studentcourse_rec[pos].course_no)){
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
	while (pos < n && atoi(newkey) > atoi(key_arr->studentcourse_rec[pos].course_no) ){
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
      printf(" %d",ptr->studentcourse_rec[i].semester);
    }
    printf("\n");
    pos = searchPosOfSemOffered(semoffered, ptr, n);
    if (pos < n && semoffered == ptr->studentcourse_rec[pos].semester){
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
  while (pos < n && newkey > key_arr->studentcourse_rec[pos].semester ){
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
    scanf("%s", e.course_no);
	printf("Enter acad_year: ");
    scanf("%s", e.acad_year);
    printf("Enter student_no: ");
    scanf("%s", e.student_no);

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
                      printf("| %d %s %s %s ", ptr->studentcourse_rec[i].id, ptr->studentcourse_rec[i].course_no,ptr->studentcourse_rec[i].acad_year,ptr->studentcourse_rec[i].student_no);
                  		
                  	switch(ptr->studentcourse_rec[i].semester){	
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

       char upCourseno[11];
       char upAcadyear[11];
       char upStudentNo[11];
       enum Semester upSemester;
       
       key = y.id;
       value = ins(root, &key, &upKey, upCourseno, upAcadyear, upStudentNo, &upSemester, &newnode, y);
      // printf("value inserted 1");
       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;
          root->studentcourse_rec[0].id = upKey;
          strcpy(root->studentcourse_rec[0].course_no,upCourseno);
          strcpy(root->studentcourse_rec[0].acad_year,upAcadyear);
          strcpy(root->studentcourse_rec[0].student_no,upStudentNo);
          root->studentcourse_rec[0].semester = upSemester;
          root->p[0] = uproot;
          root->p[1] = newnode;
         
       }else{
       }
     
}


enum KeyStatus ins(struct node *ptr, int *key, int *upKey, char *upCourseNo, char *upAcadYear, char *upStudentNo, enum Semester *upSemester, struct node** newnode, info studentcourse_rec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    int lastKey;
    char lastacad_year[50];
    char laststudent_no[50];
    char lastcourse_no[11];
    
    char ncourseno[11];
    char nacadyear[11];
    char nstudentno[11];

    enum Semester nsemester;

    enum Semester lastsem;
    enum KeyStatus value;
    int newKey;

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			*upKey = *key;

			strcpy(upCourseNo,studentcourse_rec.course_no);
  			strcpy(upAcadYear,studentcourse_rec.acad_year);
  			strcpy(upStudentNo,studentcourse_rec.student_no);
  			
  			*upSemester=studentcourse_rec.semester;

			return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos(key, ptr, n, studentcourse_rec); //studentcourse_rec = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
		
		if (pos < n && *key == ptr->studentcourse_rec[pos].id ){
			return Duplicate;
		}
		value = ins(ptr->p[pos], key, &newKey, ncourseno,nacadyear, nstudentno, &nsemester, &newPtr, studentcourse_rec);

		if (value != InsertIt){
			return value;
		}

//If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
			pos = searchPos(&newKey, ptr, n, studentcourse_rec);
			//Shifting the key and pointer right for inserting the new key/
			for (i=n; i>pos; i--){
				ptr->studentcourse_rec[i] = ptr->studentcourse_rec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			//Key is inserted at exact location
			
			ptr->studentcourse_rec[pos].id = newKey;

	        strcpy(ptr->studentcourse_rec[pos].course_no,ncourseno);
	        strcpy(ptr->studentcourse_rec[pos].acad_year,nacadyear);
  			strcpy(ptr->studentcourse_rec[pos].student_no,nstudentno);
  			ptr->studentcourse_rec[pos].semester = nsemester;

  			ptr->p[pos+1] = newPtr;
  			++ptr->n;

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			lastKey = newKey;  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
			strcpy(lastcourse_no, studentcourse_rec.course_no);
            strcpy(lastacad_year, studentcourse_rec.acad_year);
            strcpy(laststudent_no, studentcourse_rec.student_no);
            lastsem=studentcourse_rec.semester;
            
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			lastKey = ptr->studentcourse_rec[M-2].id;
			
			strcpy(lastcourse_no, ptr->studentcourse_rec[M-2].course_no);
            strcpy(lastacad_year, ptr->studentcourse_rec[M-2].acad_year);
            strcpy(laststudent_no, ptr->studentcourse_rec[M-2].student_no);
            lastsem = ptr->studentcourse_rec[M-2].semester;
            
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->studentcourse_rec[i] = ptr->studentcourse_rec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			ptr->studentcourse_rec[pos].id = newKey;
			//added the ff -mgaga
			strcpy(ptr->studentcourse_rec[pos].course_no,studentcourse_rec.course_no);
			strcpy(ptr->studentcourse_rec[pos].acad_year,studentcourse_rec.acad_year);
			strcpy(ptr->studentcourse_rec[pos].student_no, studentcourse_rec.student_no);
            ptr->studentcourse_rec[pos].semester = studentcourse_rec.semester;
            
			ptr->p[pos+1] = newPtr;
		}
		splitPos = (M - 1)/2;
        *upKey = ptr->studentcourse_rec[splitPos].id;	
        strcpy(upCourseNo,ptr->studentcourse_rec[splitPos].course_no);
        strcpy(upAcadYear,ptr->studentcourse_rec[splitPos].acad_year);
        strcpy(upStudentNo,ptr->studentcourse_rec[splitPos].student_no);
        *upSemester = ptr->studentcourse_rec[splitPos].semester;
	    
        (*newnode)=malloc(sizeof(struct node));//Right node after split
		ptr->n = splitPos; //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
			if(i < (*newnode)->n-1){
				(*newnode)->studentcourse_rec[i].id = ptr->studentcourse_rec[i + splitPos + 1].id;
				strcpy((*newnode)->studentcourse_rec[i].course_no, ptr->studentcourse_rec[i + splitPos + 1].course_no);
				strcpy((*newnode)->studentcourse_rec[i].acad_year, ptr->studentcourse_rec[i + splitPos + 1].acad_year);
				strcpy((*newnode)->studentcourse_rec[i].student_no, ptr->studentcourse_rec[i + splitPos + 1].student_no);
				(*newnode)->studentcourse_rec[i].semester = ptr->studentcourse_rec[i + splitPos + 1].semester;
			}else{
				(*newnode)->studentcourse_rec[i].id = lastKey;
				strcpy( (*newnode)->studentcourse_rec[i].course_no,lastcourse_no );
				strcpy( (*newnode)->studentcourse_rec[i].acad_year,lastacad_year );
				strcpy( (*newnode)->studentcourse_rec[i].student_no,laststudent_no );
				
				(*newnode)->studentcourse_rec[i].semester = lastsem;
			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of ins()



int searchPos(int *key, struct node *key_arr, int n, info studentcourse_rec){
	int pos=0;
	int newkey;
	newkey = *key;
	while (pos < n && newkey > key_arr->studentcourse_rec[pos].id ){
		pos++;
	}
	return pos;
	
}//End of searchPos()


int searchPosOfcourse_no(int key, struct node *key_arr, int n){
	int pos=0;
	int newkey;
	newkey = key;
	while (pos < n && newkey > key_arr->studentcourse_rec[pos].id ){
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
			printf(" %d",ptr->studentcourse_rec[i].id);
		}
		printf("\n");
		pos = searchPosOfcourse_no(key, ptr, n);
		if (pos < n && key == ptr->studentcourse_rec[pos].id){
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

	pos = searchPosOfcourse_no(key, knode, n);	

	if(p[0] == NULL){
                if (pos == n || key < knode->studentcourse_rec[pos].id)
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->studentcourse_rec[i-1] = knode->studentcourse_rec[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && key == knode->studentcourse_rec[pos].id ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->studentcourse_rec[pos] = qp->studentcourse_rec[nkey-1];
                //qp->keys[nkey - 1] = key;
		qp->studentcourse_rec[nkey-1].id = key;
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
                        rptr->studentcourse_rec[i] = rptr->studentcourse_rec[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->studentcourse_rec[0] = knode->studentcourse_rec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->studentcourse_rec[pivot] = lptr->studentcourse_rec[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
                lptr->studentcourse_rec[lptr->n] = knode->studentcourse_rec[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                knode->studentcourse_rec[pivot] = rptr->studentcourse_rec[0];
                lptr->n++;
                rptr->n--;

                for(i=0; i < rptr->n; i++){
                        rptr->studentcourse_rec[i] = rptr->studentcourse_rec[i+1];
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
        lptr->studentcourse_rec[lptr->n] = knode->studentcourse_rec[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
       
        for(i=0; i < rptr->n; i++){
                lptr->studentcourse_rec[lptr->n + 1 + i] = rptr->studentcourse_rec[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
        }

        lptr->n = lptr->n + rptr->n +1;
        free(rptr); /*Remove right node*/

        for (i=pos+1; i < n; i++){
                knode->studentcourse_rec[i-1] = knode->studentcourse_rec[i];
                p[i] = p[i+1];
        }

        return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);


}//End of del()


/*START OF FILES FUNCTIONS*/
void saveStudentData(struct node *root){
	FILE* fp=fopen("studcourse.txt","w");
	saveStData(fp,root);
	fclose(fp);
	printf("Student information are saved!\n");
}

void saveStData(FILE *fp,struct node *ptr){
	int i, j;
    if(ptr){
		for (i=0; i < ptr->n; i++){
              fprintf(fp,"%d|%s|%s|%s|%d\n",ptr->studentcourse_rec	[i].id,ptr->studentcourse_rec[i].course_no,ptr->studentcourse_rec[i].acad_year,ptr->studentcourse_rec[i].student_no,ptr->studentcourse_rec[i].semester);

		}
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			saveStData(fp, ptr->p[i]);
		}
        
	}
}

void loadStudentData(struct node **root){
	char c, line[200], *token, tok[20];
	FILE* fp=fopen("studcourse.txt","r");//reads the file
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
						case 0: x.id=atoi(token);
								break;
						case 1: strcpy(x.course_no,token);  //strcpy(tok,token)
								break;
						case 2: strcpy(x.acad_year,token);  //strcpy(tok,token)
								break;
						case 3: strcpy(x.student_no,token);  //strcy(tok,token)
								break;
						case 4: x.semester=atoi(token);
						        break;
					}
					token=strtok(NULL,"|");
					i++;

				}
				a++;
        //printf("%s %s %s %s %s %d\n", x.stdno, x.sname, x.std_bday, x.degree,  x.std_major, x.units);
				insertCourseOfferingRec(x);//place info inside the Btree
			}
			i=0;
		}
		printf("Student information are loaded! %d\n",a);
	}
	fclose(fp);
}
