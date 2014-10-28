#include<stdio.h>
#include<string.h>
#include "query_opt.h"

main(int argc, char *argsv[]){
	int i;
	QUERY_TREE qtree;

	for(i=1;i<argc;i++)
		printf("Argument %d: %s\n",i,argsv[i]);

	//get type of command
	qtree.cmd_op=getCommandType(argsv[1]);

	//get tables affected (tokenize using ';' delimeter)
}
