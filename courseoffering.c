#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "courseofferinglib.h"


int main()
{
 int choice;
 int id=0;
 int cid;
 int std_val_del;
 
 info y;
 
 loadStudentData(&root);
 printf("BTREE FOR COURSE_OFFERING\n");
 while(1){
          printf("[1] Insert\n");
          printf("[2] Delete\n");
          printf("[3] Search\n");
          printf("[4] Display\n");
          printf("[5] Quit\n");
          printf("Enter your choice :\t");
          scanf("%d", &choice);
 
          
          switch(choice){
               case 1:                   //insert
                    y=getCourseOfferingInfo(&id);
                    insertCourseOfferingRec(y);
                    break;
               
               case 2:                 //delete
                    printf("Enter the course offering id to be deleted : ");
                    scanf("%d",&cid);          
          	    DelNode(&root,cid);
                    break;
               
               case 3:	printf("SEARCH BY:\n");
               		printf("[1] Course ID \n[2] Course No \n[3] Acad Year \n[4] Max no. of Students \n[5] Time \n[6] Section\n[7] Sem offered\n");
                    	scanf("%d", &std_val_del);          
                     	search_courseoffering_data(std_val_del);
               			break;
               
               case 4:                    //display
                    printf("Btree is :\n");
                    display(root,0);
                    break;
               
               case 5:saveStudentData(root);
                    exit(1);
               
               default:
                    printf("Wrong choice\n");
                    break;
          
          }/*End of switch*/
       }/*End of while*/
       return 0;
 }/*End of main()*/
