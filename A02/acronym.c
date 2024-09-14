/*Main driver file for assignment two. This program turns all capital letters in a phrase to an acronym.
 * Gabrielle Stewart
 * September 10th, 2024
 */
#include <stdio.h>

int main() {
    char input[1024];  // initialize for input phrase
    char acronym[1024]; // initialize for acronym
    int i = 0;
    int a = 0;

    // tell user to enter a phrase
    printf("Enter a phrase: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input");
        return 1;
    }

    // create acronym
    for (i = 0; input[i] != '\0'; i++) {
        // check if this is the start of a word
        if (i == 0 || input[i - 1] == ' ') {
            // change lowercase to uppercase
            if (input[i] >= 'a' && input[i] <= 'z') {
                acronym[a++] = input[i] - ('a' - 'A');
            } else if (input[i] >= 'A' && input[i] <= 'Z') {
                acronym[a++] = input[i];
            }
        }
    }
    acronym[a] = '\0';  // null-terminate the string

    // print result :)))))
    printf("Your acronym is %s\n", acronym);

    return 0;
}
