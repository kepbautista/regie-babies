#define M 5
#define STUDENTNO 0
#define STUDENTNAME 1
#define BIRTHDAY 2
#define DEGREE 3
#define MAJOR 4
#define UNITSEARNED 5
    
typedef struct student_record{
	/*Place necessary fields here*/
	char stdno[30];
	char name[50];	
	char bday[50];	
	char degree[50];
	char major[50];
	int units;
}student;

struct node{

       student stdRec[M-1]; /*array of keys*/
       int n; /* n < M No. of keys in node will always less than order of Btree */
       struct node *p[M]; /* (n+1 pointers will be in use) */
};

struct node *root=NULL;
/*I ADDED THE FF---------------------------------------*/   

void insertStudent();
void insertStudRec(student y); 
void searchStudentNo(int key);  
int searchStudentNoPos(int key, int *key_arr, int n);
void display2(struct node *ptr,int cols[],int colnum);
/*-------------------------------------------------------*/
enum KeyStatus { Duplicate,SearchFailure,Success,InsertIt,LessKeys };

void insert2(int key);
void display(struct node *root,int);
void DelNode(struct node **root,char *x);
void search(char *x);
enum KeyStatus ins(struct node *r, char *x, char *y, struct node** u, student studRec);
int searchPos(char *x,struct node *key_arr, int n, student studRec);
enum KeyStatus del(struct node **root,struct node *ptr, char *key);
student getStudentInfo();
void search_stdno(char *x);
void search_name(char *x);
void search_bday(char *x);
void search_degree(char *x);
void search_units(int x);
int searchPosOfStdno(char *x,struct node *key_arr, int n);
int searchPosOfName(char *x,struct node *key_arr, int n);
int searchPosOfBday(char *x,struct node *key_arr, int n);
int searchPosOfDeg(char *x,struct node *key_arr, int n);
int searchPosOfUnits(int x,struct node *key_arr, int n);
/*files part*/
void saveStudentData(struct node *root);
void saveStData(FILE *fp,struct node *ptr);
void loadStudentData(struct node **root, QUERY_TREE qtree);
/*----------------------I added the ff-------------------*/
void search_std_data(x){
     char stdno[30];
     char name[100];
     char bday[50];
     char degree[50];
     char major[50];
     int units;
      if(x==1){
                       printf("Enter student number: \n"); 
                       scanf("%s",stdno);
                       search_stdno(stdno);
	                             
      }else if(x==2){
                     printf("Enter first name: \n"); 
                       scanf("%s",name);
                       search_name(name);    
                        
      }else if(x==3){
                     printf("Enter bday: \n"); 
                       scanf("%s",bday);
                       search_bday(bday); 
      }else if(x==4){
                     printf("Enter degree: \n"); 
                       scanf("%s",degree);
                       search_degree(degree); 
      }else if(x==5){
                     printf("Enter units: \n"); 
                       scanf("%d",&units);
                       search_units(units); 
      }
     
}

student getStudentInfo(){
	student e;
	
	//get student from user
    printf("Enter student no: ");
    scanf("%s", e.stdno);
	printf("Enter name: ");
    scanf("%s", e.name);
    printf("Enter bday: ");
    scanf("%s", e.bday);
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
                      printf("| %s %s %s %s %d |", ptr->stdRec[i].stdno,ptr->stdRec[i].name,ptr->stdRec[i].bday,ptr->stdRec[i].degree,ptr->stdRec[i].units);
                  //printf("%s ", ptr->stdRec[i].stdno);
            //printf(" %d:%s:%d ",ptr->studentRec.key,ptr->studentRec.studentName, ptr->studentRec.age);
		}

					
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			display(ptr->p[i], blanks+10);
		}
	}//End of if
}//End of display()



void insertStudRec(student y){
       struct node *newnode;
       char upKey[30];
       enum KeyStatus value;
       char key[30];
       
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
          strcpy(root->stdRec[0].stdno,upKey);
          strcpy(root->stdRec[0].name,y.name);
          strcpy(root->stdRec[0].bday,y.bday);
          strcpy(root->stdRec[0].degree,y.degree);
          strcpy(root->stdRec[0].major,y.major);
          root->stdRec[0].units = y.units;
          root->p[0] = uproot;
          root->p[1] = newnode;
//          printf("## %d %s %d", y.key, y.studentName, y.age);
 //         printf("### %d %d %d %s %d",(*root).n, (*root).keys[0], (*root).studentRec.key, (*root).studentRec.studentName, (*root).studentRec.age);
         
       }else{
            // printf("value not inserted");    
   //          printf("## %d %s %d", y.key, y.studentName, y.age);
   //       printf("### %d %d %d %s %d",(*root).n, (*root).keys[1], (*root).studentRec.key, (*root).studentRec.studentName, (*root).studentRec.age);
             }
     
}



enum KeyStatus ins(struct node *ptr, char *key, char *upKey,struct node **newnode,student studRec){
    struct node *newPtr, *lastPtr;       //create new instances of struct node
    int pos, i, n,splitPos;
    char lastKey[30];
    char lastName[100];
    char lastBday[50];
    char lastDeg[50];
    char lastMajor[50];
    int lastUnits;
    enum KeyStatus value;
    char newKey[30];

		if (ptr == NULL){                //if first node
			*newnode = NULL;
			//upKey = key;
			strcpy(upKey,key);
			return InsertIt;
		}

		n = ptr->n;                           //ex) 2nd insert n=1  key=2(value to be inserted) upkey = 1st key inserted 
		pos = searchPos(key, ptr, n, studRec); //studrec = y, new student from user pos=1
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
			strcpy(ptr->stdRec[pos].name,studRec.name);
      		strcpy(ptr->stdRec[pos].bday,studRec.bday);
			strcpy(ptr->stdRec[pos].degree,studRec.degree);
			strcpy(ptr->stdRec[pos].major,studRec.major);
            ptr->stdRec[pos].units = studRec.units;
			ptr->p[pos+1] = newPtr;
			++ptr->n; //incrementing the number of keys in node

			return Success;
		}//End of if 

//If keys in nodes are maximum and position of node to be inserted is last
		if (pos == M-1){
			strcpy(lastKey, newKey);  //ilipat lahat ng laman ng struct dito? +name, deg etc?
			//added the ff - mgaga
			strcpy(lastName, studRec.name);
            strcpy(lastBday, studRec.bday);
            strcpy(lastDeg, studRec.degree);
            strcpy(lastMajor, studRec.major);
            lastUnits=studRec.units;
            lastPtr = newPtr;
		
		}else{
		//If keys in node are maximum and position of node to be inserted is not last
			strcpy(lastKey,ptr->stdRec[M-2].stdno);
			//added the following -mgaga
            strcpy(lastName,ptr->stdRec[M-2].name);
            strcpy(lastBday,ptr->stdRec[M-2].bday);
            strcpy(lastDeg, ptr->stdRec[M-2].degree);
            strcpy(lastMajor, ptr->stdRec[M-2].major);
            lastUnits=ptr->stdRec[M-2].units;
            
            lastPtr = ptr->p[M-1];
            
			for (i=M-2; i>pos; i--){
				ptr->stdRec[i] = ptr->stdRec[i-1];
				ptr->p[i+1] = ptr->p[i];
			}
			strcpy(ptr->stdRec[pos].stdno,newKey);
			//added the ff -mgaga
			strcpy(ptr->stdRec[pos].name,studRec.name);
			strcpy(ptr->stdRec[pos].bday,studRec.bday);
			strcpy(ptr->stdRec[pos].degree,studRec.degree);
			strcpy(ptr->stdRec[pos].major,studRec.major);
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
				strcpy((*newnode)->stdRec[i].name, ptr->stdRec[i + splitPos + 1].name);
				strcpy((*newnode)->stdRec[i].bday, ptr->stdRec[i + splitPos + 1].bday);
				strcpy((*newnode)->stdRec[i].degree, ptr->stdRec[i + splitPos + 1].degree);
				strcpy((*newnode)->stdRec[i].major, ptr->stdRec[i + splitPos + 1].major);
				(*newnode)->stdRec[i].units = ptr->stdRec[i + splitPos + 1].units;
			}else{
				strcpy( (*newnode)->stdRec[i].stdno,lastKey );
				strcpy( (*newnode)->stdRec[i].name,lastName );
				strcpy( (*newnode)->stdRec[i].bday,lastBday );
				strcpy( (*newnode)->stdRec[i].degree,lastDeg );
				strcpy( (*newnode)->stdRec[i].major,lastMajor );
				(*newnode)->stdRec[i].units = lastUnits;
			}
		}
		(*newnode)->p[(*newnode)->n] = lastPtr;
		
		return InsertIt;
		
}//End of ins()


int searchPos(char *key, struct node *key_arr, int n, student studRec){
	int pos=0;
	char newkey[30];
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

void search_name(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->stdRec[i].name);
		}
		printf("\n");
		pos = searchPosOfName(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->stdRec[pos].name)){
			printf("First Name: %s found in position %d of last dispalyed node\n",key,i);
			return;
		}
		ptr = ptr->p[pos];
	}
	printf("Key %s is not available\n",key);
}/*End of search()*/

void search_bday(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->stdRec[i].bday);
		}
		printf("\n");
		pos = searchPosOfBday(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->stdRec[pos].bday)){
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

void search_major(char *key){
	int pos, i, n;
	struct node *ptr = root;
	printf("Search path:\n");
	
	while (ptr){
		n = ptr->n;
		for (i=0; i < ptr->n; i++){
			printf(" %s",ptr->stdRec[i].major);
		}
		printf("\n");
		pos = searchPosOfDeg(key, ptr, n);
		if (pos < n && atoi(key) == atoi(ptr->stdRec[pos].major)){
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
	printf("Key %d is not available\n",key);
}/*End of search()*/

int searchPosOfStdno(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[30];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->stdRec[pos].stdno) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfName(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[50];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->stdRec[pos].name) ){
		pos++;
	}
	return pos;
	
}

int searchPosOfBday(char *key, struct node *key_arr, int n){
	int pos=0;
	char newkey[50];
	strcpy(newkey,key);
	while (pos < n && atoi(newkey) > atoi(key_arr->stdRec[pos].bday) ){
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
	printf("Student studentrmation are saved!\n");
}


void saveStData(FILE *fp,struct node *ptr){
	int i, j;
    if(ptr){
		for (i=0; i < ptr->n; i++){
                     fprintf(fp,"%s|%s|%s|%s|%s|%d\n",ptr->stdRec[i].stdno,ptr->stdRec[i].name,ptr->stdRec[i].bday,ptr->stdRec[i].degree,ptr->stdRec[i].major,ptr->stdRec[i].units);
                  //printf("%s ", ptr->stdRec[i].stdno);
            //printf(" %d:%s:%d ",ptr->studentRec.key,ptr->studentRec.studentName, ptr->studentRec.age);
		}
		printf("\n");
		for (i=0; i <= ptr->n; i++){
			saveStData(fp, ptr->p[i]);
		}
        
	}
}

void loadStudentData(struct node **root, QUERY_TREE qtree){
	
	FILE* table_file;
	int i=0,j,k=0;
	char c, line[100], *token,*token2, tok[15];
	int insert_flag =0;
	student x;
	table_file = fopen("./db_files/student.txt","r");//create another function for loading the data
	int col_num,op;

	if(qtree.where_cond.where){ //For simple conditions
		//PUT TO ANOTHER FUNCTION LATER test first where the column belongs
		if(strcmp("StudNo",qtree.where_cond.column)==0){
			col_num=0; //throws error here, character type
		}else if(strcmp("StudentName",qtree.where_cond.column)==0){
			col_num=1; //throws error here, character type
		}else if(strcmp("Birthday",qtree.where_cond.column)==0){
			col_num=2; //throws error here, character type
		}else if(strcmp("Degree",qtree.where_cond.column)==0){
			col_num=3; //throws error here, character type
		}else if(strcmp("Major",qtree.where_cond.column)==0){
			col_num=4; //throws error here, character type
		}else if(strcmp("UnitsEarned",qtree.where_cond.column)==0){
			col_num=5; 
		}
		
		if(table_file==NULL || ((c=getc(table_file))==EOF)){
			
		//read each line of the text file
		}else{
			//Every first line on a db_file is the column it contains
				//qtree.where_cond.column			
				//qtree.where_cond.value[0]
				//qtree.where_cond.op
			ungetc(c,table_file);
			while(!feof(table_file)){
				if(fgets(line,100,table_file)!=NULL){
				
					token = strtok(line,"\n");
				//	printf("%s\n",token);
					while(token!=NULL){
						token2 = strtok(token, "|");
						j=0;//counts the number of rows
							while(token2 != NULL){
								
									switch(j){
										case 0: strcpy(x.stdno,token2);
												break;
										case 1: strcpy(x.name,token2);
												break;
										case 2: strcpy(x.bday,token2);  //strcpy(tok,token)
												break;
										case 3: strcpy(x.degree,token2);  //strcy(tok,token)
												break;
										case 4: strcpy(x.major,token2);  //strcy(tok,token)
												break;
										case 5: x.units=atoi(token2);
										        break;
									}
									
									
									if(j==col_num){
									//if on the right column, check if the cond holds true
										if(strcmp("=",qtree.where_cond.op)==0){
											if(atoi(token2)==atoi(qtree.where_cond.value[0])){
												insert_flag = 1;
											}else{
												insert_flag = 0;
											}
										}else if(strcmp(">",qtree.where_cond.op)==0){
										
											if(atoi(token2)>atoi(qtree.where_cond.value[0])){
												insert_flag = 1;
											}else{
												insert_flag = 0;
											}
										}else if(strcmp("<",qtree.where_cond.op)==0){
											if(atoi(token2)<atoi(qtree.where_cond.value[0])){
												insert_flag = 1;
											}else{
												insert_flag = 0;
											}
										}else if(strcmp("!=",qtree.where_cond.op)==0 || strcmp("<>",qtree.where_cond.op)==0){
											if(atoi(token2)!=atoi(qtree.where_cond.value[0])){
												insert_flag = 1;
											}else{
												insert_flag = 0;
											}
										}else if(strcmp("<=",qtree.where_cond.op)==0){
											if(atoi(token2)<=atoi(qtree.where_cond.value[0])){
												insert_flag = 1;
											}else{
												insert_flag = 0;
											}
										}else if(strcmp(">=",qtree.where_cond.op)==0){
											if(atoi(token2)>=atoi(qtree.where_cond.value[0])){
												insert_flag = 1;
											}else{
												insert_flag = 0;
											}
										}

									}
									
									
										
								j++;	
								token2 = strtok(NULL, "|");
							}
						
						token=strtok(NULL,"\n");
						i++;
					}
					if(insert_flag){
						insertStudRec(x);//place student inside the BST	
					}
					
				}
				i=0;
			}
			
		}
	}else if(qtree.where_cond.like){ //For like conditions (strings)
		//PUT TO ANOTHER FUNCTION LATER test first where the column belongs
		if(strcmp("StudNo",qtree.where_cond.column)==0){
			col_num=0; 
		}else if(strcmp("StudentName",qtree.where_cond.column)==0){
			col_num=1; 
		}else if(strcmp("Birthday",qtree.where_cond.column)==0){
			col_num=2; 
		}else if(strcmp("Degree",qtree.where_cond.column)==0){
			col_num=3; 
		}else if(strcmp("Major",qtree.where_cond.column)==0){
			col_num=4; 
		}else if(strcmp("UnitsEarned",qtree.where_cond.column)==0){
			col_num=5; //throws error here, UnitsEarned is int
		}
		
		if(table_file==NULL || ((c=getc(table_file))==EOF)){
			
		//read each line of the text file
		}else{
			//Every first line on a db_file is the column it contains
				//qtree.where_cond.column			
				//qtree.where_cond.value[0]
				//qtree.where_cond.op
			ungetc(c,table_file);
			while(!feof(table_file)){
				if(fgets(line,100,table_file)!=NULL){
				
					token = strtok(line,"\n");
				//	printf("%s\n",token);
					while(token!=NULL){
						token2 = strtok(token, "|");
						j=0;//counts the number of rows
							while(token2 != NULL){
								
									switch(j){
										case 0: strcpy(x.stdno,token2);
												break;
										case 1: strcpy(x.name,token2);
												break;
										case 2: strcpy(x.bday,token2);  //strcpy(tok,token)
												break;
										case 3: strcpy(x.degree,token2);  //strcy(tok,token)
												break;
										case 4: strcpy(x.major,token2);  //strcy(tok,token)
												break;
										case 5: x.units=atoi(token2);
										        break;
									}
									
									
									if(j==col_num){

									//if on the right column, check if the cond holds true
										if(qtree.where_cond.likeval[strlen(qtree.where_cond.likeval)-1]=='%' && qtree.where_cond.likeval[0]=='%'){
											//char *toked =strtok("%",where_cond.value[0]);
											/*
											if(strcmp(strstr(token2,"fam"),"fam")>0){
												printf("hello\n");	
												insert_flag = 1;
											}else{
												insert_flag = 0;
											}*/
										}else{//no modulo anywhere (equal)

											if(strcmp(token2,qtree.where_cond.likeval)==0){	

												insert_flag = 1;
											}else{
												insert_flag = 0;
											}
										}
										/*else if(strcmp(token2,qtree.where_cond.value[0])==0){	
											insert_flag = 1;
										}else{
											insert_flag = 0;
										}*/
										

									}
									
									
										
								j++;	
								token2 = strtok(NULL, "|");
							}
						
						token=strtok(NULL,"\n");
						i++;
					}
					if(insert_flag){
						insertStudRec(x);//place student inside the BST	
					}
					
				}
				i=0;
			}
			
		}
	}else{//if no condition at all
		if(table_file==NULL || ((c=getc(table_file))==EOF)){
			
		//read each line of the text file
		}else{
			//Every first line on a db_file is the column it contains
				//qtree.where_cond.column			
				//qtree.where_cond.value[0]
				//qtree.where_cond.op
			ungetc(c,table_file);
			while(!feof(table_file)){
				if(fgets(line,100,table_file)!=NULL){
				
					token = strtok(line,"\n");
				//	printf("%s\n",token);
					while(token!=NULL){
						token2 = strtok(token, "|");
						j=0;//counts the number of rows
							while(token2 != NULL){
								
									switch(j){
										case 0: strcpy(x.stdno,token2);
												break;
										case 1: strcpy(x.name,token2);
												break;
										case 2: strcpy(x.bday,token2);  //strcpy(tok,token)
												break;
										case 3: strcpy(x.degree,token2);  //strcy(tok,token)
												break;
										case 4: strcpy(x.major,token2);  //strcy(tok,token)
												break;
										case 5: x.units=atoi(token2);
										        break;
									}
									
									
									
										
								j++;	
								token2 = strtok(NULL, "|");
							}
						
						token=strtok(NULL,"\n");
						i++;
					}
					
					insertStudRec(x);//place student inside the BST	
					
					
				}
				i=0;
			}
			
		}
	}
	fclose(table_file);

}

void display2(struct node *ptr,int cols[], int colnum){
	char container[500]="";
	char buffer[30];
	if(ptr){
		int i,j;
		if(colnum==0){
			//all columns are to be projected
			for (i=0; i < ptr->n; i++){
	                      printf("%s|%s|%s|%s|%s|%d\n", ptr->stdRec[i].stdno,ptr->stdRec[i].name,ptr->stdRec[i].bday,ptr->stdRec[i].degree,ptr->stdRec[i].major,ptr->stdRec[i].units);
	                  
			}
		}else{
			//printf("here2");
			for (i=0; i < ptr->n; i++){
				for(j=0;j<colnum;j++){
					switch(cols[j]){
						case STUDENTNO:
		                	strcat(container, ptr->stdRec[i].stdno); break;
		                case STUDENTNAME:
		                	strcat(container, ptr->stdRec[i].name); break;
		                case BIRTHDAY:
		                	strcat(container, ptr->stdRec[i].bday); break;
		                case DEGREE:
		                	strcat(container, ptr->stdRec[i].degree); break;
		                case MAJOR:
		                	strcat(container, ptr->stdRec[i].major); break;
		                case UNITSEARNED:
		                	sprintf(buffer,"%d",ptr->stdRec[i].units);
		                	strcat(container,buffer ); break;
	            	}
	            	if(j==0 || j<(colnum-1))
	            	strcat(container, "|");
	            }
	            printf("%s\n",container );
	            strcpy(container,"");
			}
		}
					
		//printf("\n");
		for (i=0; i <= ptr->n; i++){
			display2(ptr->p[i],cols,colnum);
		}
	}//End of if
}//End of display()
