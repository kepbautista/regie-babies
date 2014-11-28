#define M 4
#define MAX_SIZE 100

//structures for student record
typedef struct student_record{
	char stdno[11];
	char sname[50];
	char std_bday[15];
	char degree[10];
	char std_major[10];	
	int units;
}info;
struct node{
       info stdRec[M-1];              //array of student records
       int n;                         // n < M No. of keys in node will always less than order of Btree */
       struct node *p[M];             // (n+1 pointers will be in use)
};

struct node *root=NULL;
/*-------------------------------------------------------*/
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };

//INSERT functions
info getStudentInfo();
int searchPos(char *x,struct node *key_arr, int n, info studRec);
void insertStudRec(info y);
enum KeyStatus insStudent(struct node *ptr, char *key, char *upKey,char *upSname,char *upStdBday,char *upDeg,char *upStdMajor,int *upUnits,struct node **newnode,info studRec);


//DELETE functions
void DelStdRecNode(struct node **root,char *x);
enum KeyStatus delStdRec(struct node **root,struct node *ptr, char *key);

//DISPLAY Functions
void displayStdRec(struct node *root,int);
void stud_traversal(struct node *ptr);    //display student records in ascending order

//SEARCH Functions
void search_stdno(char *x);
void search_sname(char *x);
void search_std_bday(char *x);
void search_degree(char *x);
void search_std_major(char *x);
void search_units(int x);
int searchPosOfStdno(char *x,struct node *key_arr, int n);
int searchPosOfSname(char *x,struct node *key_arr, int n);
int searchPosOfStdBday(char *x,struct node *key_arr, int n);
int searchPosOfDeg(char *x,struct node *key_arr, int n);
int searchPosOfStdMajor(char *x,struct node *key_arr, int n);
int searchPosOfUnits(int x,struct node *key_arr, int n);


//FILES Functions
void saveStudentData(struct node *root);
void saveStData(FILE *fp,struct node *ptr);
void loadStudentData(struct node **root);
void stud_traversal(struct node *root);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FUNCION DEFINITION~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* START OF INSERT FUNCTIONS*/
info getStudentInfo(){
	info e;
	
	//get info from user
    printf("Enter student no: ");
    scanf("%s", e.stdno);
    printf("Enter sname: ");
    getchar();
    fgets (e.sname, sizeof(e.sname), stdin);
    if ((strlen(e.sname)>0) && (e.sname[strlen (e.sname) - 1] == '\n'))
        e.sname[strlen (e.sname) - 1] = '\0';
    printf("Enter Birthday: ");
    scanf("%s", e.std_bday);
    printf("Enter degree: ");
    scanf("%s", e.degree);
    printf("Enter major: ");
    scanf("%s", e.std_major);
    printf("Enter units earned: ");
	scanf("%d",&e.units);

	return e;
}

void insertStudRec(info y){
       struct node *newnode;
       char upKey[11];
       char upSname[50];
       char upStdBday[15];
       char upDeg[10];
       char upStdMajor[10];
       int upUnits;

       enum KeyStatus value;
       char key[11];
       
       strcpy(key,y.stdno);
       value = insStudent(root, key, upKey,upSname,upStdBday,upDeg,upStdMajor,&upUnits, &newnode, y);
      
       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;

          strcpy(root->stdRec[0].stdno,upKey);
          strcpy(root->stdRec[0].sname,upSname);
          strcpy(root->stdRec[0].std_bday,upStdBday);
          strcpy(root->stdRec[0].degree,upDeg);
          strcpy(root->stdRec[0].std_major,upStdMajor);
          root->stdRec[0].units = upUnits;
          root->p[0] = uproot;
          root->p[1] = newnode;
       }
      
} //end of insertStudRec

enum KeyStatus insStudent(struct node *ptr, char *key, char *upKey,char *upSname,char *upStdBday,char *upDeg,char *upStdMajor,int *upUnits,struct node **newnode,info studRec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    char lastKey[11];
    char lastSname[50];
    char lastStdBday[15];
    char lastDeg[10];
    char lastStdMajor[10];
    int lastUnits;

   //char lastKey[11];
    char newKey[11];
    char nSname[50];
    char nStdBday[15];
    char nDeg[10];
    char nStdMajor[10];
    int nUnits;
    enum KeyStatus value;


		if (ptr == NULL){                //if first node
  			*newnode = NULL;
  			//upKey = key;
  			strcpy(upKey,key);
  			strcpy(upSname,studRec.sname);
  			strcpy(upStdBday,studRec.std_bday);
  			strcpy(upDeg,studRec.degree);
  			strcpy(upStdMajor,studRec.std_major);
  			*upUnits=studRec.units;
  			return InsertIt;
		}

		n = ptr->n;                            
		pos = searchPos(key, ptr, n, studRec);            //search where to add the new  node
        
		
		if (pos < n && strcmp(key, ptr->stdRec[pos].stdno)==0  ){       //if node is already in the structure
			 return Duplicate;
		}

		value = insStudent(ptr->p[pos], key, newKey,nSname,nStdBday,nDeg,nStdMajor,&nUnits, &newPtr, studRec);       //should we insert?
		
    if (value != InsertIt){
			 return value;
		}

    //If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
  			pos = searchPos(newKey, ptr, n, studRec);
  			//Shifting the key and pointer right for inserting the new key/
  			for (i=n; i>pos; i--){
            ptr->stdRec[i]=ptr->stdRec[i-1];
    				ptr->p[i+1] = ptr->p[i];
  			}
  			
        //Key is inserted at exact location
        /*I HATE THIS PART RIGHT HERE*/
  			strcpy(ptr->stdRec[pos].stdno,newKey);
        strcpy(ptr->stdRec[pos].sname,nSname);
        strcpy(ptr->stdRec[pos].std_bday,nStdBday);
  			strcpy(ptr->stdRec[pos].degree,nDeg);
  			strcpy(ptr->stdRec[pos].std_major,nStdMajor);
        ptr->stdRec[pos].units = nUnits;
  			ptr->p[pos+1] = newPtr;
  			++ptr->n;

  			return Success;
		}//End of if 

    //If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			   strcpy(lastKey, newKey);  //ilipat lahat ng laman ng struct dito? +name, deg etc?
         strcpy(lastSname, studRec.sname);
         strcpy(lastStdBday, studRec.std_bday);
         strcpy(lastDeg, studRec.degree);
         strcpy(lastStdMajor, studRec.std_major);
         lastUnits=studRec.units;

         lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			   strcpy(lastKey,ptr->stdRec[M-2].stdno);
         strcpy(lastSname,ptr->stdRec[M-2].sname);
         strcpy(lastStdBday, ptr->stdRec[M-2].std_bday);
         strcpy(lastDeg, ptr->stdRec[M-2].degree);
         strcpy(lastStdMajor,ptr->stdRec[M-2].std_major);
         lastUnits=ptr->stdRec[M-2].units;
         
         lastPtr = ptr->p[M-1];
            
    		  for(i=M-2; i>pos; i--){
              ptr->stdRec[i]=ptr->stdRec[i-1];
    				  ptr->p[i+1] = ptr->p[i];
    			}

			strcpy(ptr->stdRec[pos].stdno,newKey);
      strcpy(ptr->stdRec[pos].sname,studRec.sname);
			strcpy(ptr->stdRec[pos].std_bday,studRec.std_bday);
			strcpy(ptr->stdRec[pos].degree,studRec.degree);
			strcpy(ptr->stdRec[pos].std_major,studRec.std_major);
            ptr->stdRec[pos].units = studRec.units;

			ptr->p[pos+1] = newPtr;


		} //end of else

		splitPos = (M - 1)/2;
    
        strcpy(upKey,ptr->stdRec[splitPos].stdno);
        strcpy(upSname,ptr->stdRec[splitPos].sname);
        strcpy(upStdBday,ptr->stdRec[splitPos].std_bday);
        strcpy(upDeg,ptr->stdRec[splitPos].degree);
        strcpy(upStdMajor,ptr->stdRec[splitPos].std_major);
        *upUnits = ptr->stdRec[splitPos].units;

	  
	    
    (*newnode)=malloc(sizeof(struct node));                 //Right node after split
		ptr->n = splitPos;                                      //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;                           //No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
  			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
  			if(i < (*newnode)->n-1){
    				strcpy((*newnode)->stdRec[i].stdno, ptr->stdRec[i + splitPos + 1].stdno);
    				strcpy((*newnode)->stdRec[i].sname, ptr->stdRec[i + splitPos + 1].sname);
    				strcpy((*newnode)->stdRec[i].std_bday, ptr->stdRec[i + splitPos + 1].std_bday);
    				strcpy((*newnode)->stdRec[i].degree, ptr->stdRec[i + splitPos + 1].degree);
    				strcpy((*newnode)->stdRec[i].std_major, ptr->stdRec[i + splitPos + 1].std_major);
    				(*newnode)->stdRec[i].units = ptr->stdRec[i + splitPos + 1].units;
  			}else{
    				strcpy( (*newnode)->stdRec[i].stdno,lastKey );
    				strcpy( (*newnode)->stdRec[i].sname,lastSname );
    				strcpy( (*newnode)->stdRec[i].std_bday,lastStdBday );
    				strcpy( (*newnode)->stdRec[i].degree,lastDeg );
    				strcpy( (*newnode)->stdRec[i].std_major,lastStdMajor );
    				(*newnode)->stdRec[i].units = lastUnits;
  			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of ins()
int searchPos(char *key, struct node *key_arr, int n, info studRec){
  int pos=0;
  char newkey[11];
  strcpy(newkey,key);
  //while (pos < n && atoi(newkey) > atoi(key_arr->stdRec[pos].stdno) ){
  while (pos < n && strcmp(newkey, key_arr->stdRec[pos].stdno)>0 ){
    pos++;
  }
  return pos;
  
}


/*START OF DELETE FUNCTIONS*/
void DelStdRecNode(struct node **root, char *key){
    struct node *uproot;
    enum KeyStatus value;
  
    value = delStdRec(root,*root,key);
  
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

enum KeyStatus delStdRec(struct node **root,struct node *ptr, char *key){
    int pos, i, pivot, n ,min;
    int *key_arr;
    struct node *knode;
    int nkey;
    enum KeyStatus value;
    struct node **p,*lptr,*rptr,*qp,*qp1;

    if (ptr == NULL)
          return SearchFailure;
       
        //Assigns values of node
    n=ptr->n;
    knode = ptr;
    p = ptr->p;
    min = (M-1)/2;          //Minimum number of keys

    pos = searchPosOfStdno(key, knode, n);  

    if(p[0] == NULL){
          if (pos == n || strcmp(key,knode->stdRec[pos].stdno)<0)
                        return SearchFailure;
       
          //Shift keys and pointers left
          for(i=pos+1; i < n; i++){
                knode->stdRec[i-1] = knode->stdRec[i];
                p[i] = p[i+1];
          }
          
          return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
    }//End of if

    if (pos < n && (strcmp(key,knode->stdRec[pos].stdno)==0) ){
          qp = p[pos];
               
          while(1){
               nkey = qp->n;
               qp1 = qp->p[nkey];
               
               if (qp1 == NULL) break;                           
                
               qp = qp1;
          }//End of while

          knode->stdRec[pos] = qp->stdRec[nkey-1];      
          strcpy(qp->stdRec[nkey-1].stdno,key);
    }//End of if

    value = delStdRec(root, p[pos], key);
    
    if(value != LessKeys) return value;

    if(pos > 0 && p[pos-1]->n > min){
         pivot = pos - 1; /*pivot for left and right node*/
         lptr = p[pivot];
         rptr = p[pos];
          
          //Assigns values for right node
          rptr->p[rptr->n + 1] = rptr->p[rptr->n];
                
                for(i=rptr->n; i>0; i--){
                      rptr->stdRec[i] = rptr->stdRec[i-1];
                      rptr->p[i] = rptr->p[i-1];
                }

                rptr->n++;
                rptr->stdRec[0] = knode->stdRec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->stdRec[pivot] = lptr->stdRec[--lptr->n];
                return Success;
    }//End of if

    if(pos > min){
          pivot = pos;                  //pivot for left and right node
          lptr = p[pivot];
          rptr = p[pivot+1];

          //Assigns values for left node
          lptr->stdRec[lptr->n] = knode->stdRec[pivot];
          lptr->p[lptr->n + 1] = rptr->p[0];
          knode->stdRec[pivot] = rptr->stdRec[0];
          lptr->n++;
          rptr->n--;

          for(i=0; i < rptr->n; i++){
                rptr->stdRec[i] = rptr->stdRec[i+1];
                rptr->p[i] = rptr->p[i+1];
          }/*End of for*/
               
          rptr->p[rptr->n] = rptr->p[rptr->n + 1];
          return Success;
    }/*End of if */

    if(pos == n){
          pivot = pos-1;
    }else{
          pivot = pos;
    }
          
          lptr = p[pivot];
          rptr = p[pivot+1];


        //merge right node with left node
          lptr->stdRec[lptr->n] = knode->stdRec[pivot];
          lptr->p[lptr->n + 1] = rptr->p[0];
       
          for(i=0; i < rptr->n; i++){
                lptr->stdRec[lptr->n + 1 + i] = rptr->stdRec[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
          }

          lptr->n = lptr->n + rptr->n +1;
          free(rptr); /*Remove right node*/

          for (i=pos+1; i < n; i++){
                knode->stdRec[i-1] = knode->stdRec[i];
                p[i] = p[i+1];
          }

      return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);

}//End of del()


/*START OF SEARCH FUNCTIONS*/
void search_std_data(x){
     char stdno[11];
     char sname[50];
     char std_bday[15];
     char degree[10];
     char std_major[10];
     int units;
      if(x==1){
                       printf("Enter student number: \n"); 
                       scanf("%s",stdno);
                       search_stdno(stdno);
                               
      }else if(x==2){
                     printf("Enter Student name: \n"); 
                       scanf("%s",sname);
                       search_sname(sname); 
      }else if(x==3){
                     printf("Enter Birthday: \n"); 
                       scanf("%s",std_bday);
                       search_std_bday(std_bday); 
      }else if(x==4){
                     printf("Enter Degree: \n"); 
                       scanf("%s",degree);
                       search_degree(degree); 
      }else if(x==5){
                     printf("Enter Major: \n"); 
                       scanf("%s",std_major);
                       search_std_major(std_major); 
      }else if(x==6){
                     printf("Enter units: \n"); 
                       scanf("%d",&units);
                       search_units(units); 
      }    
}

void search_stdno(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
  		n = ptr->n;
  		
      for (i=0; i < ptr->n; i++){
  			 printf(" %s",ptr->stdRec[i].stdno);
  		}
  		printf("\n");
  		pos = searchPosOfStdno(key, ptr, n);
  		
      if (pos < n && strcmp(key,ptr->stdRec[pos].stdno)==0){
  			printf("StudentNo: %s found\n",key);
  			return;
  		}
  		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/


void search_sname(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
  		n = ptr->n;
  		for (i=0; i < ptr->n; i++){
  			printf(" %s",ptr->stdRec[i].sname);
  		}
  		printf("\n");
  		pos = searchPosOfSname(key, ptr, n);
  		
      if (pos < n && strcmp(key,ptr->stdRec[pos].sname)==0){
  			printf("Student Name: %s found\n",key);
  			return;
  		}
  		ptr = ptr->p[pos];
  	}
	  printf("Key %s is not available\n",key);
}/*End of search()*/

	void search_std_bday(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
  		n = ptr->n;
  		for (i=0; i < ptr->n; i++){
  			printf(" %s",ptr->stdRec[i].std_bday);
  		}
  		printf("\n");
  		pos = searchPosOfStdBday(key, ptr, n);
  		
      if (pos < n && strcmp(key,ptr->stdRec[pos].std_bday)==0){
  			printf("Student Bday: %s found\n",key);
  			return;
  		}
  		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_degree(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
  		n = ptr->n;
  		for (i=0; i < ptr->n; i++){
  			printf(" %s",ptr->stdRec[i].degree);
  		}
  		printf("\n");
  		pos = searchPosOfDeg(key, ptr, n);
  		
      if (pos < n && strcmp(key,ptr->stdRec[pos].degree)==0){
  			printf("Degree: %s found\n",key);
  			return;
  		}
  		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

	void search_std_major(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
  		n = ptr->n;
  		for (i=0; i < ptr->n; i++){
  			printf(" %s",ptr->stdRec[i].std_major);
  		}
  		printf("\n");
  		pos = searchPosOfSname(key, ptr, n);
  
  		if (pos < n && strcmp(key,ptr->stdRec[pos].std_major)==0){
  			printf("Student Major: %s found\n",key);
  			return;
  		}
  		ptr = ptr->p[pos];
	 }
	 printf("Key %s is not available\n",key);
}/*End of search()*/

void search_units(int key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
  		n = ptr->n;
  		for (i=0; i < ptr->n; i++){
  			printf(" %d",ptr->stdRec[i].units);
  		}
  		printf("\n");
  		pos = searchPosOfUnits(key, ptr, n);
  	
    	if (pos < n && key == ptr->stdRec[pos].units){
  			printf("Units: %d found in position %d of last dispalyed node\n",key,i);
  			return;
  		}
  		ptr = ptr->p[pos];
  	}
	  printf("Key %d is not available\n",key);
}/*End of search()*/

int searchPosOfStdno(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && strcmp(newkey, key_arr->stdRec[pos].stdno) >0 ){
		pos++;
	}
	return pos;
	
}


int searchPosOfSname(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[50];
	strcpy(newkey,key);
	while (pos < n && strcmp(newkey,key_arr->stdRec[pos].sname)>0 ){
		pos++;
	}
	return pos;
	
}

int searchPosOfStdBday(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[50];
	strcpy(newkey,key);
	while (pos < n && strcmp(newkey,key_arr->stdRec[pos].std_bday)>0 ){
		pos++;
	}
	return pos;
	
}


int searchPosOfDeg(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[50];
	strcpy(newkey,key);
	while (pos < n && strcmp(newkey,key_arr->stdRec[pos].degree)>0 ){
		pos++;
	}
	return pos;
	
}
int searchPosOfStdMajor(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[50];
	strcpy(newkey,key);
	while (pos < n && strcmp(newkey,key_arr->stdRec[pos].std_major)>0 ){
		pos++;
	}
	return pos;
	
}

int searchPosOfUnits(int key, struct node *key_arr, int n){
	int pos=0;
	int newkey;
	newkey=key;
	while (pos < n && newkey > key_arr->stdRec[pos].units ){
		pos++;
	}
	return pos;
	
}



void displayStdRec(struct node *ptr, int blanks){
      if(ptr){
      int i,j;
      for(i=1;i<=blanks;i++){
          printf(" ");
      }
      for (i=0; i < ptr->n; i++){
          printf("| %s %s %s %s %s %d |", ptr->stdRec[i].stdno,ptr->stdRec[i].sname,ptr->stdRec[i].std_bday,ptr->stdRec[i].degree,ptr->stdRec[i].std_major,ptr->stdRec[i].units);
      }
      printf("\n");
      
      for (i=0; i <= ptr->n; i++){
          displayStdRec(ptr->p[i], blanks+10);
      }
    }//End of if
}//End of display()



 void stud_traversal(struct node *ptr) {
        int i;
        if (ptr) {
                for (i = 0; i < ptr->n; i++) {
                   stud_traversal(ptr->p[i]);
                   printf("%s|%s|%s|%d\n", ptr->stdRec[i].stdno,ptr->stdRec[i].sname,ptr->stdRec[i].degree,ptr->stdRec[i].units);
                }
                stud_traversal(ptr->p[i]);
        }
  }

/*START OF FILES FUNCTIONS*/
void saveStudentData(struct node *root){
	FILE* fp=fopen("student.txt","w");
	saveStData(fp,root);
	fclose(fp);
	printf("Student information are saved!\n");
}


void saveStData(FILE *fp,struct node *ptr){
	int i, j;
    if(ptr){
		for (i=0; i < ptr->n; i++){
              fprintf(fp,"%s|%s|%s|%s|%s|%d\n",ptr->stdRec[i].stdno,ptr->stdRec[i].sname,ptr->stdRec[i].std_bday,ptr->stdRec[i].degree,ptr->stdRec[i].std_major,ptr->stdRec[i].units);

		}
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			saveStData(fp, ptr->p[i]);
		}
        
	}
}

void loadStudentData(struct node **root){
	char c, line[200], *token, tok[20];
	FILE* fp=fopen("student.txt","r");//reads the file
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
						case 0: strcpy(x.stdno,token);
								break;
						case 1: strcpy(x.sname,token);  //strcpy(tok,token)
								break;
						case 2: strcpy(x.std_bday,token);  //strcpy(tok,token)
								break;
						case 3: strcpy(x.degree,token);  //strcy(tok,token)
								break;
						case 4: strcpy(x.std_major,token);  //strcy(tok,token)
								break;
						case 5: x.units=atoi(token);
						        break;
					}
					token=strtok(NULL,"|");
					i++;

				}
				a++;
        //printf("%s %s %s %s %s %d\n", x.stdno, x.sname, x.std_bday, x.degree,  x.std_major, x.units);
				insertStudRec(x);//place info inside the Btree
			}
			i=0;
		}
		printf("Student information are loaded! %d\n",a);
	}
	fclose(fp);
}

