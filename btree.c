#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "studentlib.h"


int main()
{
 int key;
 int choice;
 info y;
 char stdno[11];
 int std_val_del;
 
 //load student record from file and save to structure
loadStudentData(&root);
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
          y=getStudentInfo();
          insertStudRec(y);
          
          break;
          
          case 2:                 //delete
              printf("Enter the student no to be deleted : ");
              scanf("%s",stdno);
	            DelStdRecNode(&root,stdno);
          break;
          
          case 3:                 //search
         // printf("Enter the tudent number : ");
      	   	  printf("1: stdno 2: lname 3: degree 4: units\n");
      	      scanf("%d", &std_val_del);            
              search_std_data(std_val_del);
          
            break;
          
          case 4:                    //display
              printf("Btree is :\n");
              displayStdRec(root,0);
          break;
          
          case 5:
              saveStudentData(root);
              exit(1);
          
          case 6:                    //display
              printf("Btree is :\n");
              stud_traversal(root);

          break;

          default:
          printf("Wrong choice\n");
          break;
          
          }/*End of switch*/
       }/*End of while*/
       return 0;
 }/*End of main()*/
