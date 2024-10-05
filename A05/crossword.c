/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: Semptember 30th, 2024
 * Description: This program generates a crossword with two words from the user. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_grid(char** grid, int rows) { //function to call in main to clear and free memory.
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

void print_grid(char** grid, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == '\0') {
                printf(". ");
            } else {
                printf("%c ", grid[i][j]);
            }
        }
        printf("\n");
    }
}

int commonletterfinder(const char* firstword, const char* secondword, int* index1, int* index2) { //common letter finder
    for (int i = 0; i < strlen(firstword); i++) {

for (int j = 0; j < strlen(secondword); j++) {
            if (firstword[i] == secondword[j]) {
                *index1 = i;
                *index2 = j;
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[]) { // main
    if (argc < 3) {
        printf("Usage: %s <firstword> <secondword>\n", argv[0]);
        return 0;
    }

    char* firstword = argv[1];
    char* secondword = argv[2];

    int index1 = -1, index2 = -1;
    if (!commonletterfinder(firstword, secondword, &index1, &index2)) {
        printf("oh no theres no common letter!\n");
        return 0;
    }

    int rows = strlen(firstword);
    int cols = strlen(secondword);

    // 2D array
    char** grid = malloc(rows * sizeof(char*));
    	for (int i = 0; i < rows; i++) {
        	grid[i] = calloc(cols, sizeof(char));
    	}

    // firstword goes vertically
    	for (int i = 0; i < rows; i++) {
        	grid[i][index2] = firstword[i];
   	 }

    // secondword goes horizontally
    	for (int j = 0; j < cols; j++) {
        	grid[index1][j] = secondword[j];
    	}

    // print out the grid!!!
    print_grid(grid, rows, cols);

    // free the allocated memory
    clear_grid(grid, rows);

    return 0;
}
