/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* width, int* height) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return NULL;
    }

    // Read the PPM file header
    char magic_number[3];
    fscanf(fp, "%2s", magic_number); // Read magic number (should be "P6")
    if (strcmp(magic_number, "P6") != 0) {
        printf("Error: Unsupported file format (not P6)\n");
        fclose(fp);
        return NULL;
    }

    // Skip any comment lines
    char ch = fgetc(fp);
    while (ch == '#') {
        while (fgetc(fp) != '\n');  // Skip to the end of the comment line
        ch = fgetc(fp);
    }
    ungetc(ch, fp);  // Put the character back for further reading

    // Read image width, height, and maximum color value
    int max_color;
    fscanf(fp, "%d %d %d", width, height, &max_color);

    if (max_color != 65535) {
        printf("Error: Unsupported max color value (not 255)\n");
        fclose(fp);
        return NULL;
    }

    fgetc(fp);  // Read the newline after the header

    // Allocate memory for the pixels
    struct ppm_pixel* pixels = (struct ppm_pixel*) malloc((*width) * (*height) * sizeof(struct ppm_pixel));
    if (pixels == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    // Read the pixel data
    size_t read_count = fread(pixels, sizeof(struct ppm_pixel), (*width) * (*height), fp);
    if (read_count != (*width) * (*height)) {
        printf("Error: Failed to read pixel data\n");
        free(pixels);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return pixels;
}

//struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  //return NULL;
//}
