/*Main driver file for assignment two. This program turns a word into an exclamation
 * Gabrielle Stewart
 * September 10th, 2024
 */
#include <stdio.h>
#include <stdlib.h>

int main() {
     char buff[32];// stores the users input in char buff[32] and processes the input from here
     char exclaim[] = {'!','*','#','@'};
     int i = 0;

//enter a word from command line
     printf("enter a word:");
//turns word into the correct size. scans until whitespace
     scanf("%31s", buff);

     printf("OMG! ");
//while loop through each character in word
     while (buff[i] != '\0') {
        if (buff[i] >= 'a' && buff[i] <= 'z') { 
	 
	     printf("%c", exclaim[rand() % 4]);//replace with the random exclamations
	  } else { 
	     printf("%c", buff[i]);//uppercase letter stay the same
	  }
	       i++;
     }
     printf("\n");
  return 0;
}
