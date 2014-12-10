#define M 5

enum Semester {First, Second, Sum};

typedef struct courseoffering_record{
	/*Place necessary fields here*/
	int id;
	char acad_year[50];
	char course_no[50];
	char section[50];
	char time[50];	
  int max_stud;
  enum Semester semester; 
    		
}info;

struct node{

       info courseOfferingRec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};

//INSERT FUNCTIONS
info get_co_info(int *id);
enum KeyStatus insert_co_courseoffering(struct node *ptr, int *key, int *upKey, char *upAcadYear, char *upCourseNo, char *upSection, char *upTime, int *upMaxStud, enum Semester *upSemOffered, struct node **newnode,info courseOfferingRec);
void insert_co_record(info y);
int searchPos_courseOffering(int *x,struct node *key_arr, int n, info courseOfferingRec);

//DELETE FUNCTIONS
void delete_co_record(struct node **root,int x);
enum KeyStatus delete_co_courseoffering(struct node **root,struct node *ptr, int key);

//DISPLAY FUNCTIONS
void display_co_record(struct node *root,int);
void display_co_asc(struct node *ptr);    //display student records in ascending order

//SEARCH FUNCTIONS
void search_co_data(int x);
int search_co_courseid(int key);
void search_co_maxstud(int key);
int search_co_semester(enum Semester semester);
int search_co_acadyear(char *key);
void search_co_time(char *key);
int search_co_courseno(char *key);
int search_co_section(char *key);
int searchPos_co_courseno(char *key, struct node *key_arr, int n);
int searchPos_co_acadyear(char *key, struct node *key_arr, int n);
int searchPos_co_section (char *key, struct node *key_arr, int n);
int searchPos_co_time(char *key, struct node *key_arr, int n);
int searchPos_co_courseid(int key, struct node *key_arr, int n);
int searchPos_co_maxstud(int key, struct node *key_arr, int n);
int searchPos_co_semoffered(enum Semester semester, struct node *key_arr, int n);
int searchDuplicate(info y);
int checkDuplicate(info y, struct node *key_arr, int n);


//FILE FUNCTIONS
void save_co_data_file(struct node *root, int id);
void save_co_data(FILE *fp,struct node *ptr);
void load_co_data_file(struct node **root, int *id);

struct node *root=NULL;

enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };

void search_co_data(int x){
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
                         search_co_courseid(id);
                         break;
                                 
        case 2: 
                      	printf("Enter course num: \n"); 
                         scanf("%s",course_no);
                         search_co_courseno(course_no); 
                         break;
        case 3:
                       printf("Enter acad year: \n"); 
                         scanf("%s",acad_year);
                         search_co_acadyear(acad_year); 
                         break;
        case 4:
                       printf("Enter Max no. of Students: \n"); 
                         scanf("%d",&max_stud);
                         search_co_maxstud(max_stud); 
                         break;

        case 5:
                       printf("Enter time: \n"); 
                         scanf("%s",time);
                         search_co_time(time); 
                         break;

        case 6:
                       printf("Enter section: \n"); 
                         scanf("%s",section);
                         search_co_section(section); 
                         break;


        case 7:         printf("Sem Offered? \n[0] 1st [1] 2nd [2] Sum\t"); 
                         scanf("%d",&so);
                         if(so==0){semester = First;}
                          else if(so==1){semester = Second;}
                            else if(so==2){semester = Sum;}
                         search_co_semester(semester);
                         break;
        default: printf("INVALID");

      }
     
}

int searchDuplicate(info y){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->courseOfferingRec[i].course_no);
    }
    printf("\n");
    pos = checkDuplicate(y, ptr, n);
    printf("%s----%s", y.course_no,ptr->courseOfferingRec[pos].course_no);
    if (pos < n && (strcmp(y.course_no,ptr->courseOfferingRec[pos].course_no)==0) && (strcmp(y.acad_year,ptr->courseOfferingRec[pos].acad_year)==0) && (strcmp(y.section,ptr->courseOfferingRec[pos].section)==0) && (y.semester==ptr->courseOfferingRec[pos].semester)){
      return 1;
    }
    ptr = ptr->p[pos];
  }
  
  return 0;
}/*End of search()*/

int checkDuplicate(info y, struct node *key_arr, int n){
  int pos=0;
  printf("\nn=%d\n", n);
  while (pos < n &&( (strcmp(y.course_no,key_arr->courseOfferingRec[pos].course_no)>0) || (strcmp(y.acad_year,key_arr->courseOfferingRec[pos].acad_year)>0) || (strcmp(y.section,key_arr->courseOfferingRec[pos].section)> 0) || (y.semester!=key_arr->courseOfferingRec[pos].semester))){
  	printf("HOOOY!");
  	printf("%s----%s\n", y.course_no,key_arr->courseOfferingRec[pos].course_no);
  	printf("%s----%s\n", y.acad_year,key_arr->courseOfferingRec[pos].acad_year);
  	printf("%s----%s\n", y.section,key_arr->courseOfferingRec[pos].section);
  	printf("%d----%d\n", y.semester,key_arr->courseOfferingRec[pos].semester);
    pos++;
  }
  return pos;
  
}

/*-------------------------------------------SEARCHES-------------------------------------------*/

int search_co_courseno(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->courseOfferingRec[i].course_no);
    }
    printf("\n");
    pos = searchPos_co_courseno(key, ptr, n);
    if (pos < n && strcmp(key,(ptr->courseOfferingRec[pos].course_no))==0){
      printf("Course No: %s found in position %d of last dispalyed node\n",key,i);
      return 1;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
  return 0;
}/*End of search()*/

int searchPos_co_courseno(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && strcmp(newkey, key_arr->courseOfferingRec[pos].course_no) > 0){
    pos++;
  }
  return pos;
  
}

int search_co_acadyear(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->courseOfferingRec[i].acad_year);
    }
    printf("\n");
    pos = searchPos_co_acadyear(key, ptr, n);
    if (pos < n &&strcmp(key, ptr->courseOfferingRec[pos].acad_year)==0){
      printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
      return 1;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
  return 0;
}/*End of search()*/

int searchPos_co_acadyear(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && strcmp(newkey, key_arr->courseOfferingRec[pos].acad_year) > 0){
    pos++;
  }
  return pos;
  
}

int search_co_section(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->courseOfferingRec[i].section);
    }
    printf("\n");
    pos = searchPos_co_section (key, ptr, n);
    if (pos < n && strcmp(key,(ptr->courseOfferingRec[pos].section))==0){
      printf("Degree: %s found in position %d of last displayed node\n",key,i);
      return 1;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
  return 0;
}/*End of search()*/

int searchPos_co_section (char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && strcmp(newkey,key_arr->courseOfferingRec[pos].section)>0){
    pos++;
  }
  return pos;
  
}

void search_co_time(char *key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %s",ptr->courseOfferingRec[i].time);
    }
    printf("\n");
    pos = searchPos_co_time (key, ptr, n);
    if (pos < n && strcmp(key,ptr->courseOfferingRec[pos].time)==0){
      printf("Degree: %s found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %s is not available\n",key);
 
}/*End of search()*/

int searchPos_co_time(char *key, struct node *key_arr, int n){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  while (pos < n && strcmp(newkey, key_arr->courseOfferingRec[pos].time)>0 ){
    pos++;
  }
  return pos;
  
}

int search_co_courseid(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->courseOfferingRec[i].id);
    }
    printf("\n");
    pos = searchPos_co_courseid(key, ptr, n);
    if (pos < n && key == ptr->courseOfferingRec[pos].id){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return 1;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
  return 0;
}/*End of search()*/

int searchPos_co_courseid(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->courseOfferingRec[pos].id ){
    pos++;
  }
  return pos;
  
}

void search_co_maxstud(int key){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->courseOfferingRec[i].max_stud);
    }
    printf("\n");
    pos = searchPos_co_maxstud(key, ptr, n);
    if (pos < n && key == ptr->courseOfferingRec[pos].max_stud){
      printf("Units: %d found in position %d of last dispalyed node\n",key,i);
      return;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",key);
}/*End of search()*/

int searchPos_co_maxstud(int key, struct node *key_arr, int n){
  int pos=0;
  int newkey;
  newkey=key;
  while (pos < n && newkey > key_arr->courseOfferingRec[pos].max_stud ){
    pos++;
  }
  return pos;
  
}


int search_co_semester(enum Semester semester){
  int pos, i, n;
  struct node *ptr = root;
  printf("Search path:\n");
  
  while (ptr){
    n = ptr->n;
    for (i=0; i < ptr->n; i++){
      printf(" %d",ptr->courseOfferingRec[i].semester);
    }
    printf("\n");
    pos = searchPos_co_semoffered(semester, ptr, n);
    if (pos < n && semester == ptr->courseOfferingRec[pos].semester){
      printf("Sem Offered: %d found in position %d of last dispalyed node\n",semester,i);
      return 1;
    }
    ptr = ptr->p[pos];
  }
  printf("Key %d is not available\n",semester);
  return 0;
}/*End of search()*/

int searchPos_co_semoffered(enum Semester semester, struct node *key_arr, int n){
  int pos=0;
  enum Semester newkey;
  newkey = semester;
  while (pos < n && newkey > key_arr->courseOfferingRec[pos].semester ){
    pos++;
  }
  return pos;
  
}

/*----------------------I added the ff-------------------*/
info get_co_info(int *cid){
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

void display_co_record(struct node *ptr, int blanks){
    if(ptr){
		int i,j;
		for(i=1;i<=blanks;i++){
			printf(" ");
		}
		for (i=0; i < ptr->n; i++){
                      printf("| %d %s %s %s %s %d ", ptr->courseOfferingRec[i].id, ptr->courseOfferingRec[i].course_no,ptr->courseOfferingRec[i].acad_year,ptr->courseOfferingRec[i].section,ptr->courseOfferingRec[i].time,ptr->courseOfferingRec[i].max_stud);
                  		
                  	switch(ptr->courseOfferingRec[i].semester){	
                  		case 0: printf("1st |"); break;
                  		case 1: printf("2nd |"); break;
                  		case 2: printf("Sum |"); break;
                  		
                  	}
		}

					
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			display_co_record(ptr->p[i], blanks+10);
		}
	}//End of if
}//End of display_co_record()

 void display_co_asc(struct node *ptr) {
        int i;
        if (ptr) {
                for (i = 0; i < ptr->n; i++) {
                   display_co_asc(ptr->p[i]);
                      printf("| %d %s %s %s %s %d ", ptr->courseOfferingRec[i].id, ptr->courseOfferingRec[i].course_no,ptr->courseOfferingRec[i].acad_year,ptr->courseOfferingRec[i].section,ptr->courseOfferingRec[i].time,ptr->courseOfferingRec[i].max_stud);
                    
                    switch(ptr->courseOfferingRec[i].semester){ 
                      case 0: printf("1st |\n"); break;
                      case 1: printf("2nd |\n"); break;
                      case 2: printf("Sum |\n"); break;
                      
                    }
                }
                display_co_asc(ptr->p[i]);
        }
  }


void insert_co_record(info y){
       struct node *newnode;
       int upKey;
       enum KeyStatus value;
       int key;
       char upAcadYear[50];
       char upCourseNo[15];
       char upSection[10];
       char upTime[10];
       int upMaxStud;
       enum Semester upSemester;
       
       key = y.id;
       value = insert_co_courseoffering(root, &key, &upKey, upAcadYear, upCourseNo, upSection, upTime, &upMaxStud, &upSemester, &newnode, y);

       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;
          root->courseOfferingRec[0].id = upKey;
          strcpy(root->courseOfferingRec[0].course_no,upCourseNo);
          strcpy(root->courseOfferingRec[0].acad_year,upAcadYear);
          strcpy(root->courseOfferingRec[0].section,upSection);
          strcpy(root->courseOfferingRec[0].time,upTime);
          root->courseOfferingRec[0].max_stud = upMaxStud;
          root->courseOfferingRec[0].semester = upSemester;
          
          root->p[0] = uproot;
          root->p[1] = newnode;
         
       }else{
       }
     
}


enum KeyStatus insert_co_courseoffering(struct node *ptr, int *key, int *upKey, char *upAcadYear, char *upCourseNo, char *upSection, char *upTime, int *upMaxStud, enum Semester *upSemester, struct node **newnode,info courseOfferingRec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    int lastKey;
    char lastacadyear[50];
    char lastsection[50];
    char lasttime[50];
    char lastcno[11];
    
    enum Semester lastsem;
    int lastmaxstud;
    int newKey;

    char nacadyear[50];
    char ncourseno[15];
    char nsection[10];
    char ntime[10];
    int nmaxstud;
    enum Semester nsemester;
    enum KeyStatus value;

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			*upKey = *key;

      strcpy(upAcadYear,courseOfferingRec.acad_year);
      strcpy(upCourseNo,courseOfferingRec.course_no);
      strcpy(upSection,courseOfferingRec.section);
      strcpy(upTime,courseOfferingRec.time);
      *upMaxStud=courseOfferingRec.max_stud;
      *upSemester=courseOfferingRec.semester;
			
      return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos_courseOffering(key, ptr, n, courseOfferingRec); //courseOfferingRec = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
		
		if (pos < n && *key == ptr->courseOfferingRec[pos].id ){
			return Duplicate;
		}

		value = insert_co_courseoffering(ptr->p[pos], key, &newKey, nacadyear, ncourseno, nsection, ntime, &nmaxstud, &nsemester, &newPtr, courseOfferingRec);

		if (value != InsertIt){
			return value;
		}

//If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
			pos = searchPos_courseOffering(&newKey, ptr, n, courseOfferingRec);
			//Shifting the key and pointer right for inserting the new key/
			for (i=n; i>pos; i--){
				ptr->courseOfferingRec[i] = ptr->courseOfferingRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			//Key is inserted at exact location
			
			ptr->courseOfferingRec[pos].id = newKey;
			strcpy(ptr->courseOfferingRec[pos].course_no,ncourseno);
			strcpy(ptr->courseOfferingRec[pos].acad_year,nacadyear);
      strcpy(ptr->courseOfferingRec[pos].section,nsection);
      strcpy(ptr->courseOfferingRec[pos].time,ntime);
			ptr->courseOfferingRec[pos].max_stud = nmaxstud;
      ptr->courseOfferingRec[pos].semester = nsemester;
			ptr->p[pos+1] = newPtr;
			++ptr->n; //incrementing the number of keys in node

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			lastKey = newKey;  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
			strcpy(lastcno, courseOfferingRec.course_no);
            strcpy(lasttime, courseOfferingRec.time);
            strcpy(lastacadyear, courseOfferingRec.acad_year);
            strcpy(lastsection, courseOfferingRec.section);
            lastmaxstud = courseOfferingRec.max_stud;
            lastsem=courseOfferingRec.semester;
            
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			lastKey = ptr->courseOfferingRec[M-2].id;
			
			strcpy(lastcno, ptr->courseOfferingRec[M-2].course_no);
            strcpy(lasttime, ptr->courseOfferingRec[M-2].time);
            strcpy(lastacadyear, ptr->courseOfferingRec[M-2].acad_year);
            strcpy(lastsection, ptr->courseOfferingRec[M-2].section);
            lastmaxstud = ptr->courseOfferingRec[M-2].max_stud;
            lastsem = ptr->courseOfferingRec[M-2].semester;
            
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->courseOfferingRec[i] = ptr->courseOfferingRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			ptr->courseOfferingRec[pos].id = newKey;
			//added the ff -mgaga
			strcpy(ptr->courseOfferingRec[pos].course_no,courseOfferingRec.course_no);
			strcpy(ptr->courseOfferingRec[pos].acad_year,courseOfferingRec.acad_year);
			strcpy(ptr->courseOfferingRec[pos].section, courseOfferingRec.section);
			strcpy(ptr->courseOfferingRec[pos].time, courseOfferingRec.time);
      ptr->courseOfferingRec[pos].max_stud = courseOfferingRec.max_stud;
      ptr->courseOfferingRec[pos].semester = courseOfferingRec.semester;
            
			ptr->p[pos+1] = newPtr;
		}
		splitPos = (M - 1)/2;
    *upKey = ptr->courseOfferingRec[splitPos].id;	
    strcpy(upAcadYear,ptr->courseOfferingRec[splitPos].acad_year);
    strcpy(upCourseNo,ptr->courseOfferingRec[splitPos].course_no);
    strcpy(upSection,ptr->courseOfferingRec[splitPos].section);
    strcpy(upTime,ptr->courseOfferingRec[splitPos].time);
    *upMaxStud = ptr->courseOfferingRec[splitPos].max_stud;
    *upSemester = ptr->courseOfferingRec[splitPos].semester;
	    
    (*newnode)=malloc(sizeof(struct node));//Right node after split
		ptr->n = splitPos; //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
			if(i < (*newnode)->n-1){
				(*newnode)->courseOfferingRec[i].id = ptr->courseOfferingRec[i + splitPos + 1].id;
				strcpy((*newnode)->courseOfferingRec[i].course_no, ptr->courseOfferingRec[i + splitPos + 1].course_no);
				strcpy((*newnode)->courseOfferingRec[i].acad_year, ptr->courseOfferingRec[i + splitPos + 1].acad_year);
				strcpy((*newnode)->courseOfferingRec[i].section, ptr->courseOfferingRec[i + splitPos + 1].section);
				strcpy((*newnode)->courseOfferingRec[i].time, ptr->courseOfferingRec[i + splitPos + 1].time);
				(*newnode)->courseOfferingRec[i].max_stud = ptr->courseOfferingRec[i + splitPos + 1].max_stud;
				(*newnode)->courseOfferingRec[i].semester = ptr->courseOfferingRec[i + splitPos + 1].semester;
			}else{
				(*newnode)->courseOfferingRec[i].id = lastKey;
				strcpy( (*newnode)->courseOfferingRec[i].course_no,lastcno );
				strcpy( (*newnode)->courseOfferingRec[i].acad_year,lastacadyear );
				strcpy( (*newnode)->courseOfferingRec[i].section,lastsection );
				strcpy( (*newnode)->courseOfferingRec[i].time,lasttime);
				
				(*newnode)->courseOfferingRec[i].max_stud = lastmaxstud;
				(*newnode)->courseOfferingRec[i].semester = lastsem;
			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of insert_co_courseoffering()



int searchPos_courseOffering(int *key, struct node *key_arr, int n, info courseOfferingRec){
	int pos=0;
	int newkey;
	newkey = *key;
	while (pos < n && newkey > key_arr->courseOfferingRec[pos].id ){
		pos++;
	}
	return pos;
	
}//End of searchPos_courseOffering()


int searchPosOfcno(int key, struct node *key_arr, int n){
	int pos=0;
	int newkey;
	newkey = key;
	while (pos < n && newkey > key_arr->courseOfferingRec[pos].id ){
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
			printf(" %d",ptr->courseOfferingRec[i].id);
		}
		printf("\n");
		pos = searchPosOfcno(key, ptr, n);
		if (pos < n && key == ptr->courseOfferingRec[pos].id){
			printf("Course Offering id: %d found in position %d of last displayed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %d is not available\n",key);
}/*End of search()*/



void delete_co_record(struct node **root, int key){
	struct node *uproot;
	enum KeyStatus value;
	
	value = delete_co_courseoffering(root,*root,key);
	
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

enum KeyStatus delete_co_courseoffering(struct node **root,struct node *ptr, int key){
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
                if (pos == n || key < knode->courseOfferingRec[pos].id)
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->courseOfferingRec[i-1] = knode->courseOfferingRec[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && key == knode->courseOfferingRec[pos].id ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->courseOfferingRec[pos] = qp->courseOfferingRec[nkey-1];
                //qp->keys[nkey - 1] = key;
		qp->courseOfferingRec[nkey-1].id = key;
        }//End of if

        value = delete_co_courseoffering(root, p[pos], key);
        if (value != LessKeys) return value;

        if (pos > 0 && p[pos-1]->n > min){
                pivot = pos - 1; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pos];
                /*Assigns values for right node*/
                rptr->p[rptr->n + 1] = rptr->p[rptr->n];
                for (i=rptr->n; i>0; i--){
                        rptr->courseOfferingRec[i] = rptr->courseOfferingRec[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->courseOfferingRec[0] = knode->courseOfferingRec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->courseOfferingRec[pivot] = lptr->courseOfferingRec[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
                lptr->courseOfferingRec[lptr->n] = knode->courseOfferingRec[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                knode->courseOfferingRec[pivot] = rptr->courseOfferingRec[0];
                lptr->n++;
                rptr->n--;

                for(i=0; i < rptr->n; i++){
                        rptr->courseOfferingRec[i] = rptr->courseOfferingRec[i+1];
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
        lptr->courseOfferingRec[lptr->n] = knode->courseOfferingRec[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
       
        for(i=0; i < rptr->n; i++){
                lptr->courseOfferingRec[lptr->n + 1 + i] = rptr->courseOfferingRec[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
        }

        lptr->n = lptr->n + rptr->n +1;
        free(rptr); /*Remove right node*/

        for (i=pos+1; i < n; i++){
                knode->courseOfferingRec[i-1] = knode->courseOfferingRec[i];
                p[i] = p[i+1];
        }

        return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);


}//End of delete_co_courseoffering()



/*START OF FILES FUNCTIONS*/
void save_co_data_file(struct node *root, int id){
  FILE* fp=fopen("courseoffering.txt","w");
  fprintf(fp,"%d\n", id);
  save_co_data(fp,root);
  fclose(fp);
  printf("Student information are saved!\n");
}

void save_co_data(FILE *fp,struct node *ptr){
  int i, j;
    if(ptr){
    for (i=0; i < ptr->n; i++){
              fprintf(fp,"%d|%s|%s|%s|%s|%d|%d\n",ptr->courseOfferingRec[i].id,ptr->courseOfferingRec[i].acad_year,ptr->courseOfferingRec[i].course_no,ptr->courseOfferingRec[i].section,ptr->courseOfferingRec[i].time,ptr->courseOfferingRec[i].max_stud, ptr->courseOfferingRec[i].semester);

    }
    printf("\n");
    for (i=0; i <= ptr->n; i++){
      save_co_data(fp, ptr->p[i]);
    }
        
  }
}

void load_co_data_file(struct node **root, int *id){
  char c, line[200], *token, tok[20];
  FILE* fp=fopen("courseoffering.txt","r");//reads the file
  info x;
  int i=0,j,a=0;//counter
  int temp;
  //if file does not exist or has no contents
  if(fp==NULL || ((c=getc(fp))==EOF)){
    printf("\nNo STUDENT RECORD yet!\n");
    *id = 0;
  }
  //read each line of the text file
  else{
    ungetc(c,fp);
    fscanf(fp, "%d\n", &temp);
    *id = temp;
    while(!feof(fp)){
      if(fgets(line,200,fp)!=NULL){
      
        //get first token
        token = strtok(line,"|");
        
        while(token!=NULL){
          switch(i){
            case 0: x.id=atoi(token);
                break;
            case 1: strcpy(x.acad_year,token);  //strcpy(tok,token)
                break;
            case 2: strcpy(x.course_no,token);  //strcpy(tok,token)
                break;
            case 3: strcpy(x.section,token);  //strcy(tok,token)
                break;
            case 4: strcpy(x.time,token);  //strcy(tok,token)
                break;
            case 5: x.max_stud=atoi(token);
                    break;
            case 6: x.semester=atoi(token);
                    break;
          }
          token=strtok(NULL,"|");
          i++;

        }
        a++;
        //printf("%s %s %s %s %s %d\n", x.stdno, x.sname, x.std_bday, x.degree,  x.std_major, x.units);
        insert_co_record(x);//place info inside the Btree
      }
      i=0;
    }
    printf("Student information are loaded! %d\n",a);
  }
  fclose(fp);

}
