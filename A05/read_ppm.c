/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: September 2024
 * Description: This program reads a PPM file, extracts the width, height, and pixel data, and returns a dynamically allocated array of struct ppm_pixel
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* width, int* height) {
    FILE* fp = fopen(filename, "rb"); // Open file in binary mode
    if (!fp) {
        fprintf(stderr, "error: Unable to open file %s\n", filename);
        return NULL;
    }

    // Read header information
    char magic[3];
    fgets(magic, sizeof(magic), fp);
    if (magic[0] != 'P' || magic[1] != '6') {
        fprintf(stderr, "Error: Unsupported PPM format (must be binary P6)\n");
        fclose(fp);
        return NULL;
    }

    // Skip comments
    char buffer[256];
    fgets(buffer, sizeof(buffer), fp);
    while (buffer[0] == '#') {
        fgets(buffer, sizeof(buffer), fp); // Keep reading until no comment
    }

    // Read width and height
    sscanf(buffer, "%d %d", width, height);

    // Read max color value (assumed to be less than 256)
    int max_val;
    fscanf(fp, "%d", &max_val);
    fgetc(fp); // Consume the newline after max color value

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
