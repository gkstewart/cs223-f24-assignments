/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

#include "write_ppm.h"
#include <stdio.h>
#include <stdlib.h>


struct ppm_pixel {
    unsigned char r;  // Red
    unsigned char g;  // Green
    unsigned char b;  // Blue
};

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    // Open the file
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return;
    }

    // PPM header
    fprintf(file, "P6\n%d %d\n255\n", w, h);

    
    int pixel_count = w * h;

    
    size_t written = fwrite(pixels, sizeof(struct ppm_pixel), pixel_count, file);
    if (written != pixel_count) {
        fprintf(stderr, "Error: Could not write all pixel data to file %s\n", filename);
    }

    fclose(file);
}

//void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

//}
