/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: October 2024
 * Description: A program that reads in a PPM file and asks the user for a message to embed.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: encode <file.ppm>\n");
        return 0;
    }

    const char* input_filename = argv[1];

    // Read the PPM file using read_ppm from your previous assignment
    Image* image = read_ppm(input_filename);
    if (image == NULL) {
        fprintf(stderr, "Error: Could not read file %s\n", input_filename);
        return 1;
    }

    int num_pixels = image->width * image->height;
    int max_chars = (num_pixels * 3) / 8;  // 3 color channels per pixel, 8 bits per character

    printf("Reading %s with width %d and height %d\n", input_filename, image->width, image->height);
    printf("Max number of characters in the image: %d\n", max_chars);

    // Get the message from the user
    char message[max_chars + 1];
    printf("Enter a phrase: ");
    fgets(message, max_chars + 1, stdin);
    int message_length = strlen(message);
    if (message[message_length - 1] == '\n') {
        message[message_length - 1] = '\0';  // Remove newline if present
        message_length--;
    }

    // Embed the message into the least significant bits of the image
    int bit_index = 0;
    for (int i = 0; i < message_length; i++) {
        char current_char = message[i];
        for (int bit = 0; bit < 8; bit++) {
            int pixel_index = bit_index / 3;
            int color_channel = bit_index % 3;

            unsigned char *color = &image->data[pixel_index * 3 + color_channel];
            *color = (*color & ~1) | ((current_char >> (7 - bit)) & 1);  // Modify LSB with message bit

            bit_index++;
        }
    }

    // Create output filename by appending "-encoded.ppm"
    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s-encoded.ppm", input_filename);

    // Write the modified image to the new file
    if (!write_ppm(output_filename, image)) {
        fprintf(stderr, "Error: Could not write to file %s\n", output_filename);
        free(image->data);
        free(image);
        return 1;
    }

    printf("Writing file %s\n", output_filename);

    // Clean up memory
    free(image->data);
    free(image);

    return 0;
}

