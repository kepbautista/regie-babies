#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include <regex.h>
#include "query_opt.h"
#include "btree2lib.h"

void executeQuery(QUERY_TREE qtree);

main(int argc, char *argsv[]) {
	int i, token_count;
	char query[30][50];
	QUERY_TREE qtree;

	// If argsv contains only one element (i.e. the executable name only), flag as error
	if(argc <= 1) {
		printf("Invalid query\n");
	}
	/**
		Arguments must be enclosed in double quotes. Accepted formats would be: 
			1. "SELECT * FROM TABLE"
			2. "SELECT" "*" "FROM" "TABLE"

		Either way, each value shall be stored in an array first, to catch both instances.
	**/
	else if(argc == 2) {
		/**
			Input format:

			"<SQL>"
		**/
		token_count = getTokens(argsv, argc, query);	
	}
	else {
		/**
			Input format:

			"<SQL>" "<SQL>" "<SQL>" ...
		**/
		token_count = getTokens(argsv, argc, query);
	}

	if(argc>=2) {
		// Now, onto tokenizing and populating the tree with the correct query
		qtree = populateQueryTree(query, token_count);

		//showQtree(qtree);
		executeQuery(qtree);
	}
}

void executeQuery(QUERY_TREE qtree){
	char container[500]="";
	int k,m;
	int cols[6]; //no of cols
	int colnum;
	if(strcmp("*",qtree.columns.colname[0])!=0){
		colnum = qtree.columns.colnum;

		//get selected columns
		for(k=0; k<qtree.columns.colnum; k++) {
			for(m=k;m<qtree.columns.colnum;m++){
				if(strcmp("StudNo",qtree.columns.colname[k])==0){
					cols[m]=0; break;
				}else if(strcmp("StudentName",qtree.columns.colname[k])==0){
					cols[m]=1; break;
				}else if(strcmp("Birthday",qtree.columns.colname[k])==0){
					cols[m]=2; break;
				}else if(strcmp("Degree",qtree.columns.colname[k])==0){
					cols[m]=3; break;
				}else if(strcmp("Major",qtree.columns.colname[k])==0){
					cols[m]=4; break;
				}else if(strcmp("UnitsEarned",qtree.columns.colname[k])==0){
					cols[m]=5; break;
				}else{
					cols[m]=6; break;
				}

			}
		}
		//generate/display TABLE HEADERS
		for(m=0;m<colnum;m++){
			strcat(container,qtree.columns.colname[m]);
			if(m==0 || m<(colnum-1))
	            strcat(container, "|");
		}
		printf("%s\n",container );
	}else{
		printf("StudNo|StudentName|Birthday|Degree|Major|UnitsEarned\n"); //only for student table yet
		colnum = 0;
	}

	switch(qtree.cmd_op) {

		case SELECT_CMD: 
			//printf("SELECT\n"); 
			//check column numbers
			//load table file for select all
				//where components :
					
			
				loadStudentData(&root,qtree); 		//select all student data,where,join
				display2(root,cols,colnum);			//display all columns,selected columns
			
			//create dynamic load for select specific columns (use display() for displaying)
			break;
		case INSERT_CMD: printf("INSERT\n"); break;
		case UPDATE_CMD: printf("UPDATE\n"); break;
		case DELETE_CMD: printf("DELETE\n"); break;
		default: printf("Wrong command given.\n");
	
	}

}
