#include<stdio.h>
#include<stdlib.h>
#include "b_tree2.h"

main(){
	int choice, x;
	BTREE *root=NULL;
	
	while((choice=menu())!=CLOSE){
		switch(choice){
			case 1: getValue(&x);
					insert(&root,x);//insert a key to the btree
					break;
			case 2: getValue(&x);
					search(root,x);//search a key in the btree
					break;
			case 3: getValue(&x);
					DelNode(&root,x);//delete a key from the btree
					break;
			case 4: printf("Btree is :\n");
					display(root,0);
					break;
			default: printf("Invalid input!\n");
		}
	}
}
