#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "courselib.h"


int main()
{
 int choice;
 int std_val_del;
 char cno[10];
 
 info y;
 
 printf("BTREE FOR COURSE\n\n");

loadStudentData(&root);
 
 while(1){
          printf("[1] Insert\n");
          printf("[2] Delete\n");
          printf("[3] Search\n");
          printf("[4] Display\n");
          printf("[5] Quit\n");
          printf("Enter your choice :\t");
          scanf("%d", &choice);
 
          
          switch(choice){
               case 1:	y=getCourseInfo();
		                insert_course(y);
		                break;
               
               case 2:	printf("Enter the student no to be deleted : ");
		                scanf("%s",cno);          
		      	     	DelNode(&root,cno);
		                break;
               
               case 3:	printf("SEARCH BY:\n");
               			printf("[1] Course Number \n[2] Course Title \n[3] Course Description \n[4] No of Units \n[5] Lab Status \n[6] Sem offered\n");
                    	scanf("%d", &std_val_del);          
                     	search_course_data(std_val_del);
               			break;
               
               case 4:	printf("Btree is :\n");
		                display(root,0);
		                break;
		           
               case 5:	saveCourseData(root);
                      exit(1);
               
               default:	printf("Wrong choice\n");
                    	break;
          }/*End of switch*/
       }/*End of while*/
       return 0;
 }/*End of main()*/
