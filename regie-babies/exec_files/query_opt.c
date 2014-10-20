#include<stdio.h>
#include "query_opt.h"

main(int argc, char *argsv[]){
	int i;

	for(i=1;i<argc;i++)
		printf("Argument %d: %s\n",i,argsv[i]);
}
