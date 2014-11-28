#define M 5

    
typedef struct studhist_record{
	/*Place necessary fields here*/
	char student_no[11];
	char description[50];	
	char action[50];	
    char date_filed[10];
   	char date_resolved[10];
	
}info;

struct node{

       info studenthist_rec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};


struct node *root=NULL;
/*I ADDED THE FF---------------------------------------*/   

void insert_studenthistory();
void insertstudenthist_rec(info y);
enum KeyStatus ins(struct node *r, char *x, char *y, char *upDescription, char *upAction, char *upDateFiled, char *upDateResolved, struct node** newnode, info studenthist_rec);
int searchPos(char *x,struct node *key_arr, int n, info studenthist_rec);
int searchPosOfstudent_no(char *x,struct node *key_arr, int n);
void display(struct node *root,int);
void DelNode(struct node **root,char *x);
enum KeyStatus del(struct node **root,struct node *ptr, char *key);
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };	
void search_sh_studentno(char *x);
void search_sh_description(char *x);
void search_sh_action(char *x);
void search_sh_datefiled(char *x);
void search_sh_dateresolved(char *x);

void saveStudentData(struct node *root);
void saveStData(FILE *fp,struct node *ptr);
void loadStudentData(struct node **root);

void search_studhist_data(x){
     char student_no[11];
     char description[50];
     char action[11];
     char date_filed[50];
     char date_resolved[50];

     switch(x){
        case 1:
                         printf("Enter student no: \n"); 
                         scanf("%s",student_no);
                         search_sh_studentno(student_no);
                         break;
                                 
        case 2: 
                       printf("Enter description: \n"); 
                         scanf("%s",description);
                         search_sh_description(description); 
                         break;
        case 3:
                       printf("Enter action: \n"); 
                         scanf("%s",action);
                         search_sh_action(action); 
                         break;
        case 4:
                       printf("Enter date filed: \n"); 
                         scanf("%s", date_filed);
                         search_sh_datefiled(date_filed); 
                         break;

       case 5:
                       printf("Enter date resolved: \n"); 
                         scanf("%s", date_resolved);
                         search_sh_dateresolved(date_resolved); 
                         break;

        default: printf("INVALID");

      }
     
}

void search_sh_studentno(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studenthist_rec[i].student_no);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studenthist_rec[pos].student_no)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_sh_description(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studenthist_rec[i].description);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studenthist_rec[pos].description)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_sh_action(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studenthist_rec[i].action);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studenthist_rec[pos].action)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_sh_datefiled(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studenthist_rec[i].date_filed);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studenthist_rec[pos].date_filed)){
			printf("StudentNo: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_sh_dateresolved(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->studenthist_rec[i].date_resolved);
		}
		printf("\n");
		pos = searchPosOfStdno(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studenthist_rec[pos].date_resolved)){
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
	while (pos < n && atoi(newkey) > atoi(key_arr->studenthist_rec[pos].student_no) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfDesc(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studenthist_rec[pos].description) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfAction(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studenthist_rec[pos].action) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfDateFiled(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studenthist_rec[pos].date_filed) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfDateResolved(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studenthist_rec[pos].date_resolved) ){
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
    scanf("%s", e.student_no);
	printf("Enter description: ");
    scanf("%s", e.description);
    printf("Enter action: ");
    scanf("%s", e.action);
    printf("Enter date filed:  ");
    scanf("%s", e.date_filed);
    printf("Enter date resolved: ");
	scanf("%s", e.date_resolved);

	
	return e;
}


void display(struct node *ptr, int blanks){
    if(ptr){
		int i,j;
		for(i=1;i<=blanks;i++){
			printf(" ");
		}
		for (i=0; i < ptr->n; i++){
                      printf("| %s %s %s %s %s |", ptr->studenthist_rec[i].student_no,ptr->studenthist_rec[i].description,ptr->studenthist_rec[i].action,ptr->studenthist_rec[i].date_filed,ptr->studenthist_rec[i].date_resolved);
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
       
       char upDescription[11];
       char upDateResolved[11];
       char upDateFiled[11];
       char upAction[11];

       strcpy(key,y.student_no);
       value = ins(root, key, upKey, upDescription, upAction, upDateFiled, upDateResolved,&newnode, y);
      // printf("value inserted 1");
       if (value == Duplicate)
          printf("Key already available\n");
       if (value == InsertIt){
          struct node *uproot = root;
          root=malloc(sizeof(struct node));
          root->n = 1;
          //root->keys[0] = upKey;
          strcpy(root->studenthist_rec[0].student_no,upKey);
          strcpy(root->studenthist_rec[0].description,upDescription);
          strcpy(root->studenthist_rec[0].action,upAction);
          strcpy(root->studenthist_rec[0].date_filed, upDateFiled);
          strcpy(root->studenthist_rec[0].date_resolved, upDateResolved);
          
          root->p[0] = uproot;
          root->p[1] = newnode;
         
       }else{
       }
     
}


enum KeyStatus ins(struct node *ptr, char *key, char *upKey, char *upDescription, char *upAction, char *upDateFiled, char *upDateResolved, struct node** newnode, info studenthist_rec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    char lastKey[11];
    char lastdescription[50];
    char lastaction[50];
    char lastdate_filed[10];
    char lastdate_resolved[10];
    enum KeyStatus value;
    char newKey[11];

    char ndescription[50];
    char naction[50];
    char ndatefiled[50];
    char ndateresolved[50];

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			strcpy(upKey,key);
			strcpy(upDescription,studenthist_rec.description);
  			strcpy(upAction,studenthist_rec.action);
  			strcpy(upDateFiled,studenthist_rec.date_filed);
  			strcpy(upDateResolved,studenthist_rec.date_resolved);
			return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos(key, ptr, n, studenthist_rec); //studenthist_rec = y, new info from user pos=1
        //printf("\nPOS CHECK\n: %d",pos);
		
		if (pos < n && strcmp(key, ptr->studenthist_rec[pos].student_no)==0  ){
			return Duplicate;
		}

		value = ins(ptr->p[pos], key, newKey, ndescription, naction, ndatefiled, ndateresolved ,&newPtr, studenthist_rec);

		if (value != InsertIt){
			return value;
		}

//If keys in node is less than M-1 where M is order of B tree
		if (n < M - 1){
			pos = searchPos(newKey, ptr, n, studenthist_rec);
			//Shifting the key and pointer right for inserting the new key/
			for (i=n; i>pos; i--){
				ptr->studenthist_rec[i] = ptr->studenthist_rec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			//Key is inserted at exact location
			strcpy(ptr->studenthist_rec[pos].student_no,newKey);
			strcpy(ptr->studenthist_rec[pos].description,ndescription);
			strcpy(ptr->studenthist_rec[pos].action,naction);
			strcpy(ptr->studenthist_rec[pos].date_filed,ndatefiled);
			strcpy(ptr->studenthist_rec[pos].date_resolved,ndateresolved);
			ptr->p[pos+1] = newPtr;
			++ptr->n; //incrementing the number of keys in node

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			strcpy(lastKey, newKey);  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
			strcpy(lastdescription, studenthist_rec.description);
            strcpy(lastaction, studenthist_rec.action);
            strcpy(lastdate_filed, studenthist_rec.date_filed);
            strcpy(lastdate_resolved, studenthist_rec.date_resolved);
            
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			strcpy(lastKey,ptr->studenthist_rec[M-2].student_no);
			//added the following -mgaga
            strcpy(lastdescription,ptr->studenthist_rec[M-2].description);
            strcpy(lastaction,ptr->studenthist_rec[M-2].action);
            strcpy(lastdate_filed, ptr->studenthist_rec[M-2].date_filed);
            strcpy(lastdate_resolved, ptr->studenthist_rec[M-2].date_resolved);
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->studenthist_rec[i] = ptr->studenthist_rec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			strcpy(ptr->studenthist_rec[pos].student_no,newKey);
			//added the ff -mgaga
			strcpy(ptr->studenthist_rec[pos].description,studenthist_rec.description);
			strcpy(ptr->studenthist_rec[pos].action,studenthist_rec.action);
			strcpy(ptr->studenthist_rec[pos].date_filed, studenthist_rec.date_filed);
            strcpy(ptr->studenthist_rec[pos].date_resolved, studenthist_rec.date_resolved);
            
			ptr->p[pos+1] = newPtr;
		}
		splitPos = (M - 1)/2;

        strcpy(upKey,ptr->studenthist_rec[splitPos].student_no);
        strcpy(upDescription,ptr->studenthist_rec[splitPos].description);
        strcpy(upAction,ptr->studenthist_rec[splitPos].action);
        strcpy(upDateFiled,ptr->studenthist_rec[splitPos].date_filed);
        strcpy(upDateResolved,ptr->studenthist_rec[splitPos].date_resolved);
	  //  printf("UPKEY: %s, %s",upKey, ptr->studenthist_rec[splitPos].student_no);		
	    
        (*newnode)=malloc(sizeof(struct node));//Right node after split
		ptr->n = splitPos; //No. of keys for left splitted node
		(*newnode)->n = M-1-splitPos;//No. of keys for right splitted node
		
		for (i=0; i < (*newnode)->n; i++){
			(*newnode)->p[i] = ptr->p[i + splitPos + 1];
			if(i < (*newnode)->n-1){
				strcpy((*newnode)->studenthist_rec[i].student_no, ptr->studenthist_rec[i + splitPos + 1].student_no);
				strcpy((*newnode)->studenthist_rec[i].description, ptr->studenthist_rec[i + splitPos + 1].description);
				strcpy((*newnode)->studenthist_rec[i].action, ptr->studenthist_rec[i + splitPos + 1].action);
				strcpy((*newnode)->studenthist_rec[i].date_filed, ptr->studenthist_rec[i + splitPos + 1].date_filed);
				strcpy((*newnode)->studenthist_rec[i].date_resolved, ptr->studenthist_rec[i + splitPos + 1].date_resolved);
			}else{
				strcpy( (*newnode)->studenthist_rec[i].student_no,lastKey );
				strcpy( (*newnode)->studenthist_rec[i].description,lastdescription );
				strcpy( (*newnode)->studenthist_rec[i].action,lastaction );
				
				strcpy((*newnode)->studenthist_rec[i].date_resolved, lastdate_resolved);
				strcpy((*newnode)->studenthist_rec[i].date_filed, lastdate_filed);
			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of ins()

int searchPos(char *key, struct node *key_arr, int n, info studenthist_rec){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studenthist_rec[pos].student_no) ){
		pos++;
	}
	return pos;
	
}//End of searchPos()

int searchPosOfstudent_no(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[11];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->studenthist_rec[pos].student_no) ){
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
			printf(" %s",ptr->studenthist_rec[i].student_no);
		}
		printf("\n");
		pos = searchPosOfstudent_no(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->studenthist_rec[pos].student_no)){
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

	pos = searchPosOfstudent_no(key, knode, n);	

	if(p[0] == NULL){
                if (pos == n || (atoi(key) < atoi(knode->studenthist_rec[pos].student_no)))
                        return SearchFailure;
       
                //Shift keys and pointers left
                for(i=pos+1; i < n; i++){
                        knode->studenthist_rec[i-1] = knode->studenthist_rec[i];
                        p[i] = p[i+1];
                }
       		
                return --ptr->n >= (ptr==*root ? 1 : min) ? Success : LessKeys;
        }//End of if

        if (pos < n && (atoi(key) == atoi(knode->studenthist_rec[pos].student_no)) ){
                qp = p[pos];
               
                while(1){
                        nkey = qp->n;
                        qp1 = qp->p[nkey];
               
                        if (qp1 == NULL) break;
                               
                        qp = qp1;
                }//End of while

                knode->studenthist_rec[pos] = qp->studenthist_rec[nkey-1];
                //qp->keys[nkey - 1] = key;
		strcpy(qp->studenthist_rec[nkey-1].student_no,key);
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
                        rptr->studenthist_rec[i] = rptr->studenthist_rec[i-1];
                        rptr->p[i] = rptr->p[i-1];
                }
                rptr->n++;
                rptr->studenthist_rec[0] = knode->studenthist_rec[pivot];
                rptr->p[0] = lptr->p[lptr->n];
                knode->studenthist_rec[pivot] = lptr->studenthist_rec[--lptr->n];
                return Success;
        }/*End of if */

        if (pos > min){
                pivot = pos; /*pivot for left and right node*/
                lptr = p[pivot];
                rptr = p[pivot+1];

                /*Assigns values for left node*/
                lptr->studenthist_rec[lptr->n] = knode->studenthist_rec[pivot];
                lptr->p[lptr->n + 1] = rptr->p[0];
                knode->studenthist_rec[pivot] = rptr->studenthist_rec[0];
                lptr->n++;
                rptr->n--;

                for(i=0; i < rptr->n; i++){
                        rptr->studenthist_rec[i] = rptr->studenthist_rec[i+1];
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
        lptr->studenthist_rec[lptr->n] = knode->studenthist_rec[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
       
        for(i=0; i < rptr->n; i++){
                lptr->studenthist_rec[lptr->n + 1 + i] = rptr->studenthist_rec[i];
                lptr->p[lptr->n + 2 + i] = rptr->p[i+1];
        }

        lptr->n = lptr->n + rptr->n +1;
        free(rptr); /*Remove right node*/

        for (i=pos+1; i < n; i++){
                knode->studenthist_rec[i-1] = knode->studenthist_rec[i];
                p[i] = p[i+1];
        }

        return (--ptr->n >= (ptr == *root ? 1 : min) ? Success : LessKeys);


}//End of del()


/*START OF FILES FUNCTIONS*/
void saveStudentData(struct node *root){
	FILE* fp=fopen("studhist.txt","w");
	saveStData(fp,root);
	fclose(fp);
	printf("Student information are saved!\n");
}

void saveStData(FILE *fp,struct node *ptr){
	int i, j;
    if(ptr){
		for (i=0; i < ptr->n; i++){
              fprintf(fp,"%s|%s|%s|%s|%s\n",ptr->studenthist_rec[i].student_no,ptr->studenthist_rec[i].description,ptr->studenthist_rec[i].action,ptr->studenthist_rec[i].date_filed,ptr->studenthist_rec[i].date_resolved);

		}
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			saveStData(fp, ptr->p[i]);
		}
        
	}
}

void loadStudentData(struct node **root){
	char c, line[200], *token, tok[20];
	FILE* fp=fopen("studhist.txt","r");//reads the file
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
						case 0: strcpy(x.student_no,token);  //strcpy(tok,token)
								break;
						case 1: strcpy(x.description,token);  //strcpy(tok,token)
								break;
						case 2: strcpy(x.action,token);  //strcy(tok,token)
								break;
						case 3: strcpy(x.date_filed,token);  //strcy(tok,token)
								break;
						case 4: strcpy(x.date_resolved,token);  //strcy(tok,token)
								break;
					}
					token=strtok(NULL,"|");
					i++;

				}
				a++;
        //printf("%s %s %s %s %s %d\n", x.stdno, x.sname, x.std_bday, x.degree,  x.std_major, x.units);
				insertCourseRec(x);//place info inside the Btree
			}
			i=0;
		}
		printf("Student information are loaded! %d\n",a);
	}
	fclose(fp);
}