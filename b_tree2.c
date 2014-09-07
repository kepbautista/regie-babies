#include<stdio.h>
#include<stdlib.h>
#include "b_tree2.h"

main(){
	int choice, x;
	BTREE *root=NULL;
	
	while((choice=menu())!=CLOSE){
		switch(choice){
			case 1: x=getValue();
					insert(&root,x);//insert a key to the btree
					printf("(%d)",root->nodes);
					break;
			case 2: x=getValue();
					search(root,x);//search a key in the btree
					break;
			case 3: x=getValue();
					DelNode(&root,x);//delete a key from the btree
					break;
			case 4: printf("Btree is :\n");
					display(root,0);
					break;
			default: printf("Invalid input!\n");
		}
	}
}
