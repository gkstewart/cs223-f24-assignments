/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: October 2024
 * Description: This program reads in a PPm file ans outputs any message stored in any part of color.   
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  char* filename = argv[1];
    int width, height;
    struct ppm_pixel* pixels = read_ppm(filename, &width, &height);

    if (pixels == NULL) {
        printf("Error: Could not read file %s\n", filename);
        return 1;
    }

    printf("Reading %s with width %d and height %d\n", filename, width, height);

    // find the  max number of characters (3 bits per pixel -> 1 character per 8 bits)
    int max_chars = (width * height * 3) / 8;
    printf("Max number of characters in the image: %d\n", max_chars);

    char* message = (char*) malloc(max_chars + 1);
    if (message == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    int char_index = 0;
    int bit_index = 0;
    char current_char = 0;

    for (int i = 0; i < width * height * 3; i++) {
        unsigned char* pixel_bytes = (unsigned char*) pixels;
        unsigned char lsb = pixel_bytes[i] & 0x1; //  least significant bit

        current_char |= (lsb << bit_index);
        bit_index++;

        if (bit_index == 8) {  // 8 bits = 1 character
            message[char_index] = current_char;

            if (current_char == '\0') {
                break;  
            }

            char_index++;
            current_char = 0;
            bit_index = 0;
        }
    }

    message[char_index] = '\0'; 

    // Output the decoded message
    printf("%s\n", message);

    // Free memory
    free(pixels);
    free(message);

  return 0;
}

