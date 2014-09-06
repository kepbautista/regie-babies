#include<stdio.h>
#include<stdlib.h>
#include "b_tree.h"

main(){
	int choice, x, i;
	BTREE *root=NULL, *p;
	
	while((choice=menu())!=CLOSE){
		switch(choice){
			case 1: x = getValue();
					insertValue(&root,x);
					break;
			case 2: x = getValue();
					p = search(root,x);
					break;
			case 3: printBTREE(root,1);
					break;
			case 4: printf("Exit\n");
					break;
			default: printf("Invalid Input!\n");
		}
	}
}
