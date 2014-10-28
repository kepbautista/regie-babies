//commands
#define SELECT_CMD 1
#define INSERT_CMD 2
#define UPDATE_CMD 3
#define DELETE_CMD 4

//conditions
#define GREATER_THAN 1
#define LESS_THAN 2

//structure for conditions
typedef struct 
{
	/* data */
	char leftcol[30];
	char rightcol[30];
	char cond_op;
} COND;

//Structure for the Query Tree
typedef struct
{
	/* data */
	int cmd_op;//command or operation
	char columns[30][50];//sets of columns
	COND select_cond;
	COND join_cond;
	char tables[30][50];
} QUERY_TREE;

//determine type of command
int getCommandType(char *cmd){
	if(strcmp(cmd,"SELECT")==0) return SELECT_CMD;
	if(strcmp(cmd,"INSERT")==0) return INSERT_CMD;
	if(strcmp(cmd,"UPDATE")==0) return UPDATE_CMD;
	if(strcmp(cmd,"DELETE")==0) return DELETE_CMD;
}

void execute_query(){

}
