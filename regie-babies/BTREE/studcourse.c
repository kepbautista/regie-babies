#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "studcourselib.h"


int main()
{
 int choice;
 info y;
 int id=0;
 int scid;
 int std_val_del;
 printf("Creation of B tree for node %d\n",M);
 load_sc_data_file(&root, &id);
 while(1){
          printf("1.Insert\n");
          printf("2.Delete\n");
          printf("3.Search\n");
          printf("4.Display\n");
          printf("5.Quit\n");
          printf("6.Display in asc\n");
          printf("Enter your choice : ");
          scanf("%d",&choice);
 
         
          switch(choice){
               case 1:                   //insert
                    y=get_sc_info(&id);
                    insert_sc_record(y);
                    break;
               
               case 2:                 //delete
                    printf("Enter the course offering id to be deleted : ");
                    scanf("%d",&scid);          
                    delete_sc_record(&root,scid);
                    break;
               
               case 3:                 //search
                    printf("1: cno 2: stdno 3: acadyear 4: id 5: semester\n");
                    scanf("%d", &std_val_del);          
                   
                    search_sc_data(std_val_del);
               
               case 4:                    //display
                    printf("Btree is :\n");
                    display_sc_record(root,0);
                    break;
               
               case 5: save_sc_data_file(root, id);
                    exit(1);

               case 6:                    //display
              printf("Btree is :\n");
              display_sc_asc(root);
              break;
               
               default:
                    printf("Wrong choice\n");
                    break;
         
          }/*End of switch*/
       }/*End of while*/
       return 0;
 }/*End of main()*/

