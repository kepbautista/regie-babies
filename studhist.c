#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "studhistlib.h"


int main()
{
 int choice;
 info y;
 char shid[10];
 int std_val_del;
 printf("Creation of B tree for node %d\n",M);

 loadStudentData(&root);
 while(1){
          printf("1.Insert\n");
          printf("2.Delete\n");
          printf("3.Search\n");
          printf("4.Display\n");
          printf("5.Quit\n");
          printf("Enter your choice : ");
          scanf("%d",&choice);
 
          
          switch(choice){
               case 1:                   //insert
                    y=getStudHistInfo();
                    insertCourseRec(y);
                    break;
               
               case 2:                 //delete
                    printf("Enter the student no to be deleted : ");
                    scanf("%s",shid);          
          	    DelNode(&root,shid);
                    break;
               
               case 3:                 //search
                    printf("1: stdno 2: description 3: action 4: dateFiled 5: dateResolved \n");
                    scanf("%d", &std_val_del);          
                    
                    search_studhist_data(std_val_del);
                    break;
               
               case 4:                    //display
                    printf("Btree is :\n");
                    display(root,0);
                    break;
               
               case 5: saveStudentData(root);
                    exit(1);
               
               default:
                    printf("Wrong choice\n");
                    break;
          
          }/*End of switch*/
       }/*End of while*/
       return 0;
 }/*End of main()*/
