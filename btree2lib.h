#define M 5

    
typedef struct student_record{
	/*Place necessary fields here*/
	char stdno[11];
	char sname[50];	
	char degree[50];	
	int units;
}info;

struct node{

       info stdRec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};

struct node *root=NULL;
/*I ADDED THE FF---------------------------------------*/   

void insertStudent();
void insertStudRec(info y); 
void searchStudentNo(int key);  
int searchStudentNoPos(int key, int *key_arr, int n);

/*-------------------------------------------------------*/
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };

void insert2(int key);
void display(struct node *root,int);
void DelNode(struct node **root,char *x);
void search(char *x);
enum KeyStatus ins(struct node *r, char *x, char *y, struct node** u, info studRec);
int searchPos(char *x,struct node *key_arr, int n, info studRec);
enum KeyStatus del(struct node **root,struct node *ptr, char *key);
info getStudentInfo();
void search_stdno(char *x);
void search_sname(char *x);
void search_degree(char *x);
void search_units(int x);
int searchPosOfStdno(char *x,struct node *key_arr, int n);
int searchPosOfSname(char *x,struct node *key_arr, int n);
int searchPosOfDeg(char *x,struct node *key_arr, int n);
int searchPosOfUnits(int x,struct node *key_arr, int n);
/*files part*/
void saveStudentData(struct node *root);
void saveStData(FILE *fp,struct node *ptr);
void loadStudentData(struct node **root);
/*----------------------I added the ff-------------------*/
void search_std_data(x){
     char stdno[11];
     char sname[50];
     char degree[50];
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
                     printf("Enter degree: \n"); 
                       scanf("%s",degree);
                       search_degree(degree); 
      }else if(x==4){
                     printf("Enter units: \n"); 
                       scanf("%d",&units);
                       search_units(units); 
      }
     
}

info getStudentInfo(){
	info e;
	
	//get info from user
    printf("Enter student no: ");
    scanf("%s", e.stdno);
    printf("Enter sname: ");
    scanf("%s", e.sname);
    printf("Enter degree: ");
    scanf("%s", e.degree);
    printf("Enter units earned: ");
	scanf("%d",&e.units);

	return e;
}

void display(struct node *ptr, int blanks){
    if(ptr){
		int i,j;
		for(i=1;i<=blanks;i++){
			printf(" ");
		}
		for (i=0; i < ptr->n; i++){
                      printf("| %s %s %s %d |", ptr->stdRec[i].stdno,ptr->stdRec[i].sname,ptr->stdRec[i].degree,ptr->stdRec[i].units);
                  //printf("%s ", ptr->stdRec[i].stdno);
            //printf(" %d:%s:%d ",ptr->studentRec.key,ptr->studentRec.studentName, ptr->studentRec.age);
		}

					
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			display(ptr->p[i], blanks+10);
		}
	}//End of if
}//End of display()



void insertStudRec(info y){
       struct node *newnode;
       char upKey[11];
       enum KeyStatus value;
       char key[11];
       
       strcpy(key,y.stdno);
       value = ins(root, key, upKey, &newnode, y);
      
       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;
         
          strcpy(root->stdRec[0].stdno,upKey);
          strcpy(root->stdRec[0].sname,y.sname);
          strcpy(root->stdRec[0].degree,y.degree);
          root->stdRec[0].units = y.units;
          root->p[0] = uproot;
          root->p[1] = newnode;
       
       }
     
}



enum KeyStatus ins(struct node *ptr, char *key, char *upKey,struct node **newnode,info studRec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    char lastKey[11];
    char lastSname[50];
    char lastDeg[50];
    int lastUnits;
    enum KeyStatus value;
    char newKey[11];

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			//upKey = key;
			strcpy(upKey,key);
			return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos(key, ptr, n, studRec); //studrec = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
		
		if (pos < n && strcmp(key, ptr->stdRec[pos].stdno)==0  ){
			return Duplicate;
		}
		value = ins(ptr->p[pos], key, newKey, &newPtr, studRec);

		if (value != InsertIt){
			return value;
		}

//If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
			pos = searchPos(newKey, ptr, n, studRec);
			//Shifting the key and pointer right for inserting the new key/
			for (i=n; i>pos; i--){
				ptr->stdRec[i] = ptr->stdRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			//Key is inserted at exact location
			strcpy(ptr->stdRec[pos].stdno,newKey);
      		strcpy(ptr->stdRec[pos].sname,studRec.sname);
			strcpy(ptr->stdRec[pos].degree,studRec.degree);
            ptr->stdRec[pos].units = studRec.units;
			ptr->p[pos+1] = newPtr;
			++ptr->n; //incrementing the number of keys in node

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			strcpy(lastKey, newKey);  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
            strcpy(lastSname, studRec.sname);
            strcpy(lastDeg, studRec.degree);
            lastUnits=studRec.units;
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			strcpy(lastKey,ptr->stdRec[M-2].stdno);
			//added the following -mgaga
            strcpy(lastSname,ptr->stdRec[M-2].sname);
            strcpy(lastDeg, ptr->stdRec[M-2].degree);
            lastUnits=ptr->stdRec[M-2].units;
            
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->stdRec[i] = ptr->stdRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			strcpy(ptr->stdRec[pos].stdno,newKey);
			//added the ff -mgaga
			strcpy(ptr->stdRec[pos].sname,studRec.sname);
			strcpy(ptr->stdRec[pos].degree,studRec.degree);
            ptr->stdRec[pos].units = studRec.units;
            
			ptr->p[pos+1] = newPtr;
		}
		splitPos = (M - 1)/2;
	//	(*upKey) = ptr->keys[splitPos];
	 //   printf("SPLITPOS: %d",splitPos);
        strcpy(upKey,ptr->stdRec[splitPos].stdno);
	  //  printf("UPKEY: %s, %s",upKey, ptr->stdRec[splitPos].stdno);		
	    
        (*newnode)=malloc(sizeof(struct node));//Right node after split
		ptr->n = splitPos; //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
			if(i < (*newnode)->n-1){
				strcpy((*newnode)->stdRec[i].stdno, ptr->stdRec[i + splitPos + 1].stdno);
				strcpy((*newnode)->stdRec[i].sname, ptr->stdRec[i + splitPos + 1].sname);
				strcpy((*newnode)->stdRec[i].degree, ptr->stdRec[i + splitPos + 1].degree);
				(*newnode)->stdRec[i].units = ptr->stdRec[i + splitPos + 1].units;
			}else{
				strcpy( (*newnode)->stdRec[i].stdno,lastKey );
				strcpy( (*newnode)->stdRec[i].sname,lastSname );
				strcpy( (*newnode)->stdRec[i].degree,lastDeg );
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
	while (pos < n && atoi(newkey) > atoi(key_arr->stdRec[pos].stdno) ){
		pos++;
	}
	return pos;
	
}//End of searchPos()


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

	pos = searchPosOfStdno(key, knode, n);	

	if(p[0] == NULL){
                if (pos == n || (atoi(key) < atoi(knode->stdRec[pos].stdno)))
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->stdRec[i-1] = knode->stdRec[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && (atoi(key) == atoi(knode->stdRec[pos].stdno)) ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->stdRec[pos] = qp->stdRec[nkey-1];
                //qp->keys[nkey - 1] = key;
		strcpy(qp->stdRec[nkey-1].stdno,key);
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
                        rptr->stdRec[i] = rptr->stdRec[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->stdRec[0] = knode->stdRec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->stdRec[pivot] = lptr->stdRec[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
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

        if(pos == n)
                pivot = pos-1;
        else
                pivot = pos;

        lptr = p[pivot];
        rptr = p[pivot+1];


        /*merge right node with left node*/
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
		if (pos < n && atoi(key) == atoi(ptr->stdRec[pos].stdno)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
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
		if (pos < n && atoi(key) == atoi(ptr->stdRec[pos].sname)){
			printf("Last Name: %s found in position %d of last dispalyed node\n",key,i);
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
		if (pos < n && atoi(key) == atoi(ptr->stdRec[pos].degree)){
			printf("Degree: %s found in position %d of last dispalyed node\n",key,i);
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
	printf("Key %s is not available\n",key);
}/*End of search()*/

int searchPosOfStdno(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->stdRec[pos].stdno) ){
		pos++;
	}
	return pos;
	
}


int searchPosOfSname(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[50];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->stdRec[pos].sname) ){
		pos++;
	}
	return pos;
	
}


int searchPosOfDeg(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[50];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->stdRec[pos].degree) ){
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



/*===================================FILES========================================*/

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
                     fprintf(fp,"%s %s %s %d\n",ptr->stdRec[i].stdno,ptr->stdRec[i].sname,ptr->stdRec[i].degree,ptr->stdRec[i].units);
                  //printf("%s ", ptr->stdRec[i].stdno);
            //printf(" %d:%s:%d ",ptr->studentRec.key,ptr->studentRec.studentName, ptr->studentRec.age);
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
	int i=0,j;//counter
	
	//if file does not exist or has no contents
	if(fp==NULL || ((c=getc(fp))==EOF))
		printf("\nNo STUDENT RECORD yet!\n");
	//read each line of the text file
	else{
		ungetc(c,fp);
		while(!feof(fp)){
			if(fgets(line,200,fp)!=NULL){
			
				//get first token
				token = strtok(line," ");
			  
				while(token!=NULL){
					switch(i){
						case 0: strcpy(x.stdno,token);
								break;
						case 1: strcpy(x.sname,token);  //strcpy(tok,token)
								break;
						case 2: strcpy(x.degree,token);  //strcy(tok,token)
								break;
						case 3: x.units=atoi(token);
						        break;
					}
					token=strtok(NULL," ");
					i++;
				}
			//	x.bday=toDate(tok);//convert date values to integer
				
				insertStudRec(x);//place info inside the BST
			}
			i=0;
		}
		printf("Student information are loaded!\n");
	}
	fclose(fp);
}

