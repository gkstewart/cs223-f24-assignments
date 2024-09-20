/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: September 16th, 2024
 * Description: This program turns a phrase into ent-speech.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // define the variables pauseLength and userText
    int pauseLength;
    char userText[32];

    // ask the pause length from the user
    printf("pause length? ");
    scanf("%d", &pauseLength);

    // ask for the text from the user
    printf("text? ");
    scanf("%s", userText);  

    // find the length of the user text
    int textLength = strlen(userText);

    // calculating how long the new string will be
    // each letter except the last one has 'pauseLength' . after
    //  new size is textLength + (textLength - 1) * pauseLength + 1 (for '\0')
    int newSize = textLength + (textLength - 1) * pauseLength + 1;

    // allocate memory for  "ent-speech" string
    char *entSpeech = (char *)malloc(newSize * sizeof(char));
    if (entSpeech == NULL) {
        printf("error: could not allocate memory\n");
        return 1;  
    }

    //  the new string with pauses
    int currentIndex = 0;
    for (int i = 0; i < textLength; i++) {
        // copy the letters
        entSpeech[currentIndex] = userText[i];
        currentIndex++;

        // add dots after each letter
        if (i < textLength - 1) {
            for (int j = 0; j < pauseLength; j++) {
                entSpeech[currentIndex] = '.';
                currentIndex++;
            }
        }
    }

    // add the null character
    entSpeech[currentIndex] = '\0';

    // print line for  ent-speech string
    printf("%s\n", entSpeech);

    // free the memory 
    free(entSpeech);

    return 0; 
}
