/**
QUERY FORMATS THAT SHOULD BE ALLOWED:

SELECT <*[col1,col2,...]> FROM <table1,table2,...>
INSERT INTO <table_name> VALUES(val1,val2,val3...)
UPDATE <table_name> SET <[col1=val1,col2=val2,col3=val3,...]>
DELETE FROM <table_name>

A flag is added to the WHERE structure. Why? Because the following cases could occur:

SELECT * FROM TABLE WHERE COL=VAL => 'stand-alone' form, even if JOIN command follows
SELECT * FROM TABLE JOIN TABLE2 ON VAL=VAL WHERE COL=VAL => 'subquery' of JOIN
UPDATE TABLE SET COL=VAL WHERE COL=VAL => 'subquery' of UPDATE
DELETE FROM TABLE WHERE COL=VAL => 'subquery' of DELETE

And then there're BETWEEN

...WHERE colname BETWEEN val1 AND val2
...WHERE colname NOT BETWEEN val1 AND val2

LIKE:

...WHERE colname LIKE '%searchkey%'
...WHERE colname LIKE '%searchkey'
...WHERE colname LIK 'searchkey%'

IN

...WHERE colname IN (val1, val2, val3,...)
**/
/*I ADDED THE FF---------------------------------------*/   

// Commands
#define SELECT_CMD 1
#define INSERT_CMD 2
#define UPDATE_CMD 3
#define DELETE_CMD 4

// Where operators
#define EQ 1
#define NEQ1 2 // The <> combination
#define NEQ2 3 // The != combination
#define LT 4
#define GT 5
#define LTE 6
#define GTE 7

int i, counter, join_index, where_index, set_index, del_index, sel_index, between_index, like_index, in_index;
const char space[2] = " ";
const char comma[2] = ",";
const char eq[2] = "=";
const char delimiters[] = "(,) ;";
char *token;
char *whereClause;
char *joinClause;
char values[30][50];

typedef struct {
	/* For the values after the SET command */
	char column[30][50];
	char value[30][50];
	int colnum;
} SET_EXP;

typedef struct {
	/* For the values after the WHERE command */
	char column[30];
	char value[30][50];
	char invalues[30][50];
	int in_valnum;

	char op[6];
	char likeval[30];

	char errline[30]; //Part with error
	/**
		The following are flags to indicate what would be done for WHERE. 1 for TRUE, otherwise FALSE
	**/
	int sub_of_select; // The where is stated before join
	int sub_of_join; // The where expression is stated after the join
	int sub_of_update; // The where is part of update
	int sub_of_delete; // The where is part of delete
	int not_between; // The where has a NOT BETWEEN command
	int between; // The where has a BETWEEN command
	int like; // The where has a LIKE query
	int in; // The where has an IN query.
	int where; // Simplest form of where: col operator value is given
	int error; //Flag for error?
} WHERE_EXP;

/**
	Under WHERE you have:
		col operator value (E.g. =,<,>) -> single value can be saved in value[0]
		col [not] between val1 and val2 -> between values can be saved in value[0] and value[1], flag between will be 1 (not_between, if the key NOT is included in the query)
		col in (val1,val2,val3) -> values will be saved in value[0] up to value[n], flag in will be 1
		col like 'key' -> search key can be saved in value[0], flag like will be 1
		
**/

typedef struct {
	/* For the values after the JOIN command */
	int has_join;
	char tablename[30]; // Just one table for join for now
	char leftval[30];
	char rightval[30];
} JOIN_EXP;

typedef struct {
	/* Column studentrmation */
	char colname[30][50]; //sets of columns
	int colnum; // indicates how many columns are being queried
} COLUMN;

typedef struct {
	/* Table studentrmation */
	char tablename[30][50]; //sets of columns
	int tablenum; // indicates how many columns are being queried
} TABLE;

typedef struct {
	/* Values to be inserted */
	char ins_val[30][50];
	int ins_num;
} INS;

//Structure for the Query Tree
typedef struct {
	/* data */
	int cmd_op; //command or operation
	COLUMN columns;
	TABLE tables;	
	WHERE_EXP where_cond;
	JOIN_EXP join_cond;
	INS insert_cond;
	SET_EXP set_cond;
} QUERY_TREE;

QUERY_TREE qtree;

//determine type of command
int getCommandType(char *cmd){
	/**
		 Changed strcmp to strcasecmp so that even if the user typed in 'Select', 'select' or 'SELECT', it would be recognized by the parser. Should checks be made, though, on the format of the word itself? (Meaning, only the above three forms are allowed, everything else shall be treated as a syntax error.) If the format and the case are of importance, we can use regex for this, instead of strcmp or strcasecmp 
	**/
	if(strcasecmp(cmd,"SELECT")==0) return SELECT_CMD;
	if(strcasecmp(cmd,"INSERT")==0) return INSERT_CMD;
	if(strcasecmp(cmd,"UPDATE")==0) return UPDATE_CMD;
	if(strcasecmp(cmd,"DELETE")==0) return DELETE_CMD;
}

// Pre-processing step: whether the format of the parameter string is "query" or <"command" "command" "command">, it'll be accepted
int getTokens(char *sql_query[], int state, char query[30][50]) {
	// Separate by space
	if(state==2) {
		token = strtok(sql_query[1], space);
	   	counter = 0;
		// Iterate through tokens
		while( token != NULL ) 
		{
			strcpy(query[counter],token);		    		
			counter++;
		      	token = strtok(NULL, space);
		}
		return counter;
	}
	else {
		for(i=1; i<state; i++) {
			strcpy(query[i-1], sql_query[i]);
		}
		counter = state-1;
		return counter;
	}
}

// Tokenize the WHERE clause using regex. At least, if the user inputs any other combination of symbols, the program would automatically detect it as invalid, if it's not included in the allowed list.

char *getExpOp(char *argument) {
	int status;
	int state;
	char *expOp;

	char *pattern[7];


	pattern[0] = "[a-zA-Z0-9]+={1}[a-zA-Z0-9]+";
	pattern[1] = "[a-zA-Z0-9]+<{1}>{1}[a-zA-Z0-9]+";
	pattern[2] = "[a-zA-Z0-9]+!{1}={1}[a-zA-Z0-9]+";
	pattern[3] = "[a-zA-Z0-9]+<{1}[a-zA-Z0-9]+";
	pattern[4] = "[a-zA-Z0-9]+>{1}[a-zA-Z0-9]+";
	pattern[5] = "[a-zA-Z0-9]+<{1}={1}[a-zA-Z0-9]+";
	pattern[6] = "[a-zA-Z0-9]+>{1}={1}[a-zA-Z0-9]+";

	regex_t re;

	for(i=0; i<7; i++) {
		if (regcomp(&re, pattern[i], REG_EXTENDED|REG_NOSUB) != 0) {
			state = 0;
		}
		status = regexec(&re, argument, (size_t) 0, NULL, 0);
		if (status == 0) {
			state=i+1;      
		}
		regfree(&re);
	}
	
	switch(state) {
		case EQ: expOp = "="; break;
		case NEQ1: expOp = "<>"; break;
		case NEQ2: expOp = "!="; break;
		case LT: expOp = "<"; break;
		case GT: expOp = ">"; break;
		case LTE: expOp = "<="; break;
		case GTE: expOp = ">="; break;
		case 0: expOp = "invalid"; break;
		default: break;
	}

	return expOp;
}

// Populates the query tree.
// NOTE: The tokenizing part seems to be repetitive. Better create a function for that to make the code cleaner, if it's possible
QUERY_TREE populateQueryTree(char query[30][50], int token_count) {
	// Get the main command
	qtree.cmd_op = getCommandType(query[0]);

	if(qtree.cmd_op==SELECT_CMD) {
		/**
			SYNTAX:
			SELECT <*[col1,col2,...]>
			FROM <table1,table2,...>
			WHERE some_column=some_value;
			
			TOKENS:
			query[0] = SELECT
			query[1] = columns or *
			query[2] = FROM
			query[3] = table(s)
			query[4] = WHERE
			query[5] = some_column=some_value
		**/
		// Store the columns that would be queried
		token = strtok(query[1], comma);
	   	counter = 0;
		// Iterate through tokens
		while( token != NULL ) 
	   	{
			strcpy(qtree.columns.colname[counter],token);		    		
			counter++;
	      		token = strtok(NULL, comma);
	   	}
		qtree.columns.colnum = counter;

		// Store tables that would be queried
		token = strtok(query[3], comma);
	   	counter = 0;
		// Iterate through tokens
		while( token != NULL ) 
	   	{
			strcpy(qtree.tables.tablename[counter],token);		    		
			counter++;
	      		token = strtok(NULL, comma);
	   	}
		qtree.tables.tablenum = counter;
	}
	else if(qtree.cmd_op==INSERT_CMD) {
		/**
			SYNTAX:
			INSERT 
			INTO table_name || INTO table_name (col1,col2,...)
			VALUES (val1,val2,...)
			
			TOKENS:
			
			Format 1:
			query[0] = INSERT
			query[1] = INTO
			query[2] = table_name
			query[3] = VALUES
			query[4] = (val1,val2,val3,...)

			Format 2:
			query[0] = INSERT
			query[1] = INTO
			query[2] = table_name
			query[3] = (col1, col2, col3,...)
			query[4] = VALUES
			query[5] = (val1,val2,val3,...)

			
		**/	
		strcpy(qtree.tables.tablename[0],query[2]);

		if(strcasecmp(query[3],"VALUES")!=0) {
			
			// There are columns specified
			token = strtok(query[3], delimiters);
			counter = 0;
			// Iterate through tokens
			while( token != NULL ) 
			{
				strcpy(values[counter],token);
				counter++;		    		
				token = strtok(NULL, delimiters);
			}
			qtree.columns.colnum = counter;

			for(i=0; i<counter; i++) {
				strcpy(qtree.columns.colname[i],values[i]);
			}
		}

		
		// Get the individual values within the VALUES string
		token = strtok(query[token_count-1], delimiters);
		counter = 0;
		// Iterate through tokens
		while( token != NULL ) 
		{
			strcpy(values[counter],token);
			counter++;		    		
			token = strtok(NULL, delimiters);
		}
		qtree.insert_cond.ins_num = counter;

		for(i=0; i<counter; i++) {
			strcpy(qtree.insert_cond.ins_val[i],values[i]);
		}
	}
	else if(qtree.cmd_op==UPDATE_CMD) {
		/**
			SYNTAX:
			UPDATE table_name
			SET column1=value1,column2=value2,...
			WHERE some_column=some_value;
			
			TOKENS:
			query[0] = UPDATE
			query[1] = table_name
			query[2] = SET
			query[3] = column1=value1,column2=value2,...
			query[4] = WHERE
			query[5] = some_column=some_value
		**/	
		strcpy(qtree.tables.tablename[0],query[1]);
		
		// First, tokenize queryp[3] by using a comma, and save each token into an initial array. Then, for each array value, get the columns and values
		char set_values[30][50];
		token = strtok(query[3], comma);
		counter = 0;
		// Iterate through tokens
		while( token != NULL ) 
		{
			strcpy(set_values[counter],token); // This should contain the ['col1=val1'],['col2=val2'],...
			counter++;		    		
			token = strtok(NULL, comma);
		}

		
		qtree.set_cond.colnum = counter;
		counter = 0;
		char temp_storage[50][50];

		for(i=0; i<qtree.set_cond.colnum; i++) {
			token = strtok(set_values[i], eq);
			
			// Iterate through tokens
			while( token != NULL ) 
			{
				strcpy(temp_storage[counter],token);		    		
				token = strtok(NULL, eq);
				counter++;
			}
		}
		int index1 =0;
		int index2 = 0;
		int tracker = 0;
		while(tracker < counter) {
			if(tracker%2==0) {
				strcpy(qtree.set_cond.column[index1],temp_storage[tracker]);
				index1++;
			}
			if(tracker%2!=0) {
				strcpy(qtree.set_cond.value[index2],temp_storage[tracker]);
				index2++;
			}
			tracker++;
		}
	}
	else if(qtree.cmd_op==DELETE_CMD) {
		/**
			SYNTAX:
			DELETE FROM table_name
			WHERE some_column=some_value;
			
			TOKENS:
			query[0] = DELETE
			query[1] = FROM
			query[2] = table_name
			query[3] = WHERE
			query[4] = some_column=some_value
		**/	
		strcpy(qtree.tables.tablename[0],query[2]);
	}
		
	// We have to iterate through the tokens and see if there is a where and/or a join clause within the query
	for(i=0; i<token_count+1; i++) {
		if(strcasecmp(query[i], "SELECT")==0) {
			sel_index = i;
		}
		if(strcasecmp(query[i], "WHERE")==0) {
			where_index = i;
		}
		if(strcasecmp(query[i],"JOIN")==0) {
			join_index = i;
			qtree.join_cond.has_join = 1;
		}
		if(strcasecmp(query[i],"SET")==0) {
			set_index = i;
		}
		if(strcasecmp(query[i],"DELETE")==0) {
			del_index = i;
		}
		if(strcasecmp(query[i],"BETWEEN")==0) {
			between_index = i;
		}
		if(strcasecmp(query[i],"LIKE")==0) {
			like_index = i;
		}
		if(strcasecmp(query[i],"IN")==0) {
			in_index = i;
		}		
	}


		// Get the tokens for the WHERE clause and/or JOIN clause
		// Since we now have the indices where the JOIN and the WHERE clauses start, we can easily assume that the indices following these commands belong to them (provided that there would be no nested querying)
		// If there is a 'WHERE' clause found
	if(where_index) {
		
		if(join_index) {
			if(where_index > sel_index && where_index<join_index) qtree.where_cond.sub_of_select=1;
			else if(where_index > join_index) qtree.where_cond.sub_of_join=1; // This means that the WHERE came after the JOIN clause, and thus is part of the JOIN
		}
		
		if(set_index) {
			qtree.where_cond.sub_of_update=1; // This means that the WHERE came after the SET clause, and thus is part of the SET
		}

		if(del_index==0) {
			qtree.where_cond.sub_of_delete=1;
		}
		
		if(between_index || like_index || in_index) {
		/**
			...WHERE column BETWEEN val1 AND val2
		**/
			if(between_index) {
				if(strcasecmp(query[between_index-1],"NOT") != 0) {
					qtree.where_cond.between = 1;
				}
				else {
					qtree.where_cond.not_between = 1;
				}
			
				strcpy(qtree.where_cond.column,query[where_index+1]);
				strcpy(qtree.where_cond.value[0],query[between_index+1]);
				strcpy(qtree.where_cond.value[1],query[between_index+3]);
			}
			if(like_index) {
				/**
				...WHERE column LIKE "%searchkey%"
				**/
				// Get the individual values within the VALUES string

				qtree.where_cond.like = 1;
				strcpy(qtree.where_cond.column,query[where_index+1]);
				strcpy(qtree.where_cond.likeval,query[like_index+1]);

			}
			if(in_index) {
				qtree.where_cond.in = 1;
				/**
				...WHERE column IN (val1,val2,val3,...)
				**/
				// Get the individual values within the VALUES string
			
				strcpy(qtree.where_cond.column,query[where_index+1]);
				token = strtok(query[in_index+1], delimiters);
				counter = 0;
				// Iterate through tokens
				while( token != NULL ) 
				{
					strcpy(values[counter],token);
					counter++;		    		
					token = strtok(NULL, delimiters);
				}
				qtree.where_cond.in_valnum = counter;

				for(i=0; i<counter; i++) {
					strcpy(qtree.where_cond.invalues[i],values[i]);
				}
			}
			
		}
		else {
			qtree.where_cond.where=1;
			whereClause = query[where_index+1]; // It is certain that the expression after 'WHERE' belongs to it

			char *whereOp = getExpOp(whereClause);
			
			if (strcmp(whereOp,"invalid") != 0) {
				token = strtok(whereClause, whereOp);
				counter = 0;
				while( token != NULL )	{
					strcpy(values[counter],token);		    		
					counter++;
				   	token = strtok(NULL, whereOp);
			 	}
				strcpy(qtree.where_cond.column,values[0]);
				strcpy(qtree.where_cond.value[0],values[1]);
				strcpy(qtree.where_cond.op,whereOp);
			}
			else {
				qtree.where_cond.error = 100;
				strcpy(qtree.where_cond.errline,whereClause);
			}
		}
	}
	else {
		strcpy(qtree.where_cond.column,"EMPTY");
		strcpy(qtree.where_cond.value[0],"EMPTY");
		strcpy(qtree.where_cond.op,"EMPTY");
	}

		// If there is a 'JOIN' clause found. Will only work with ONE JOIN query.
		
		// We know that the index after 'JOIN' is a table (or a set of tables? Must check later), so we could just parse that one and save it in the join structure
// ... JOIN table ON val=val
	if(join_index) {
		joinClause = query[join_index+3];
		char *joinOp = getExpOp(joinClause);
		if (strcmp(joinOp,"invalid") != 0) {
			strcpy(qtree.join_cond.tablename,query[join_index+1]);

			token = strtok(joinClause, joinOp);
			counter = 0;
			while( token != NULL )	{
				strcpy(values[counter],token);		    		
				counter++;
			   	token = strtok(NULL, joinOp);
		 	}
			strcpy(qtree.join_cond.leftval,values[1]);
			strcpy(qtree.join_cond.rightval,values[0]);
		}
		else {
			qtree.where_cond.error = 101;
			strcpy(qtree.where_cond.errline,joinClause);
		}
	}
	else {
		strcpy(qtree.join_cond.tablename,"EMPTY");
		strcpy(qtree.join_cond.leftval,"EMPTY");
		strcpy(qtree.join_cond.rightval,"EMPTY");
	}


	return qtree;
}

void showQtree(QUERY_TREE qtree) {
	if(qtree.where_cond.error) {
		if(qtree.where_cond.error==100) {
			printf("ERROR 100: Invalid WHERE syntax\n");
			printf("%s\n", qtree.where_cond.errline);
		}
		if(qtree.where_cond.error==101) {
			printf("ERROR 100: Invalid JOIN syntax\n");
			printf("%s\n", qtree.where_cond.errline);
		}
	}
	else {
		printf("\nMAIN COMMAND ISSUED:\n");
		switch(qtree.cmd_op) {
			case SELECT_CMD: printf("SELECT\n"); break;
			case INSERT_CMD: printf("INSERT\n"); break;
			case UPDATE_CMD: printf("UPDATE\n"); break;
			case DELETE_CMD: printf("DELETE\n"); break;
			default: printf("Wrong command given.\n");
		}

		if(qtree.cmd_op==SELECT_CMD) {	
			printf("\nCOLUMN(S) QUERIED:\n");
			for(i=0; i<qtree.columns.colnum; i++) {	
				printf("%s\n", qtree.columns.colname[i]);
			}

			printf("\nTABLE(S) QUERIED:\n");
			for(i=0; i<qtree.tables.tablenum; i++) {	
				printf("%s\n", qtree.tables.tablename[i]);
			}
		}
		else if(qtree.cmd_op==INSERT_CMD){
			// Show values to be inserted
			printf("\nTABLE(S) QUERIED:\n");
			printf("%s\n", qtree.tables.tablename[0]);

			if(qtree.columns.colnum) {
				printf("\nCOLUMN(S) QUERIED:\n");
				for(i=0; i<qtree.columns.colnum; i++) {
					printf("%s\n", qtree.columns.colname[i]);
				}
			}

			printf("\nVALUES TO BE INSERTED:\n");
			for(i=0; i<qtree.insert_cond.ins_num; i++) {	
				printf("%s\n", qtree.insert_cond.ins_val[i]);
			}
		}
		else if(qtree.cmd_op==UPDATE_CMD){
			// Show values to be updated
			printf("\nTABLE(S) QUERIED:\n");
			printf("%s\n", qtree.tables.tablename[0]);

			printf("\nSET\n");
			for(i=0; i<qtree.set_cond.colnum; i++) {
				printf("%s to %s\n", qtree.set_cond.column[i], qtree.set_cond.value[i]);
			}
		}
		else if(qtree.cmd_op==DELETE_CMD){
			// Show delete query
			printf("\nTABLE(S) QUERIED:\n");
			printf("%s\n", qtree.tables.tablename[0]);
		}

		if(qtree.where_cond.sub_of_select==1) {

			printf("\nWHERE CLAUSE:\n");
			printf("COLUMN: %s\n", qtree.where_cond.column);
			printf("VALUE: %s\n", qtree.where_cond.value[0]);
			printf("OPERATOR: %s\n", qtree.where_cond.op);


			printf("\nJOIN CLAUSE:\n");
			printf("TABLE JOINED: %s\n", qtree.join_cond.tablename);
			printf("LEFT VALUE: %s\n", qtree.join_cond.rightval);
			printf("RIGHT VALUE: %s\n", qtree.join_cond.leftval);
		}
		else if(qtree.where_cond.sub_of_join==1) {
			printf("\nJOIN CLAUSE:\n");
			printf("TABLE JOINED: %s\n", qtree.join_cond.tablename);
			printf("LEFT VALUE: %s\n", qtree.join_cond.rightval);
			printf("RIGHT VALUE: %s\n", qtree.join_cond.leftval);

			printf("\nWHERE CLAUSE:\n");
			printf("COLUMN: %s\n", qtree.where_cond.column);
			printf("VALUE: %s\n", qtree.where_cond.value[0]);
			printf("OPERATOR: %s\n", qtree.where_cond.op);
		}
		else if(qtree.join_cond.has_join==1) {
			printf("\nJOIN CLAUSE:\n");
			printf("TABLE JOINED: %s\n", qtree.join_cond.tablename);
			printf("LEFT VALUE: %s\n", qtree.join_cond.rightval);
			printf("RIGHT VALUE: %s\n", qtree.join_cond.leftval);
		}
		else if(qtree.where_cond.between==1) {
			printf("\nWHERE CLAUSE: BETWEEN\n");
			printf("COLUMN: %s\n", qtree.where_cond.column);
			
			printf("FIRST VALUE: %s\n", qtree.where_cond.value[0]);
			printf("SECOND VALUE: %s\n", qtree.where_cond.value[1]);
		}
		else if(qtree.where_cond.not_between==1) {
			printf("\nWHERE CLAUSE: NOT BETWEEN\n");
			printf("COLUMN: %s\n", qtree.where_cond.column);
			
			printf("FIRST VALUE: %s\n", qtree.where_cond.value[0]);
			printf("SECOND VALUE: %s\n", qtree.where_cond.value[1]);
		}
		else if(qtree.where_cond.like==1) {
			printf("\nWHERE CLAUSE:\n");
			printf("COLUMN: %s\n", qtree.where_cond.column);
			printf("VALUE: %s\n", qtree.where_cond.likeval);
		}
		else if(qtree.where_cond.in==1){
			printf("\nWHERE CLAUSE:\n");
			printf("COLUMN: %s\n", qtree.where_cond.column);
			printf("VALUES TO BE CHECKED AGAINST:\n");
			for(i=0; i<qtree.where_cond.in_valnum; i++) {
				printf("%s\n", qtree.where_cond.invalues[i]);
			}
		}
		else if(qtree.where_cond.where==1){
			printf("\nWHERE CLAUSE:\n");
			printf("COLUMN: %s\n", qtree.where_cond.column);
			printf("VALUE: %s\n", qtree.where_cond.value[0]);
			printf("OPERATOR: %s\n", qtree.where_cond.op);
		}
	}
}
/*
typedef struct {
	// data 
	int cmd_op; //command or operation
	COLUMN columns;
	TABLE tables;	
	WHERE_EXP where_cond;
	JOIN_EXP join_cond;
	INS insert_cond;
	SET_EXP set_cond;
} QUERY_TREE;
*//*
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

void insertStudRec(student y){
       struct node *newnode;
       char upKey[11];
       enum KeyStatus value;
       char key[11];
       
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


void load(struct node **root){

	FILE* table_file;//reads the file
	int i=0,j,k=0;//counter
	char c, line[100], *token,*token2, tok[15];
	
	student x;
	//path_to_table = strcat(path_to_table,"student");
	//path_to_table = strcat(path_to_table,".txt");
	table_file = fopen("./db_files/student.txt","r");//create another function for loading the data

	if(table_file==NULL || ((c=getc(table_file))==EOF)){
		
	//read each line of the text file
	}else{
		//Every first line on a db_file is the column it contains
		
		ungetc(c,table_file);
		while(!feof(table_file)){
			if(fgets(line,100,table_file)!=NULL){
			
				token = strtok(line,"\n");
			//	printf("%s\n",token);
				while(token!=NULL){
					token2 = strtok(token, "|");
					j=0;//counts the number of rows
						while(token2 != NULL){
							for(k=0;k<6;k++){ //k counts the number of column
								switch(k){
										/*char stdno[50];
										char name[100];	
										char bday[50];
										char degree[50];
										char major[50];
										int units;*//*
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
	fclose(table_file);

}
*/



