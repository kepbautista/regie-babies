#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "courseofferinglib.h"


int main()
{
 int choice, id=0;
 int cid;
 int std_val_del;
 
 info y;
 
 load_co_data_file(&root, &id);
 printf("BTREE FOR COURSE_OFFERING\n");
 while(1){
          printf("[1] Insert\n");
          printf("[2] Delete\n");
          printf("[3] Search\n");
          printf("[4] Display\n");
          printf("[5] Quit\n");
          printf("6.Display in asc\n");
          printf("Enter your choice :\t");
          scanf("%d", &choice);
 
          
          switch(choice){
               case 1:                   //insert
                    y=get_co_info(&id);
                    insert_co_record(y);
                    break;
               
               case 2:                 //delete
                    printf("Enter the course offering id to be deleted : ");
                    scanf("%d",&cid);          
          	    delete_co_record(&root,cid);
                    break;
               
               case 3:	printf("SEARCH BY:\n");
               		printf("[1] Course ID \n[2] Course No \n[3] Acad Year \n[4] Max no. of Students \n[5] Time \n[6] Section\n[7] Sem offered\n");
                    	scanf("%d", &std_val_del);          
                     	search_co_data(std_val_del);
               			break;
               
               case 4:                    //display
                    printf("Btree is :\n");
                    display_co_record(root,0);
                    break;
               
               case 5:save_co_data_file(root, id);
                    exit(1);

               case 6:                    //display
              printf("Btree is :\n");
              display_co_asc(root);
              break;
               
               default:
                    printf("Wrong choice\n");
                    break;
          
          }/*End of switch*/
       }/*End of while*/
       return 0;
 }/*End of main()*/
