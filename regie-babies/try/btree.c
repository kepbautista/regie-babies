#include<stdio.h>
#include<stdlib.h>
#include "btree.h"

main(){
	int choice;
	student_tuple x;
	BTREE *root=NULL;

	while((choice=printMenu())!=EXIT_PROGRAM){
		switch(choice){
			case 1: x = getStudentValue();
					
					insertNode(&root,x);
					break;
			case 2: //printNodeValues(root);
					//printNodeValues(root->node_ptr[0]);
					printBTREE(root);
					break;
			case 3: printf("Program closed!\n");
					break;
			default: printf("Invalid Input!\n");
		}
	}
}
