/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: September 2024
 * Description: writes the pixel data into a new PPM file in binary format
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int width, int height) {
	FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return 1;
    }
    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    
    fwrite(pixels, sizeof(struct ppm_pixel), width * height, fp);

    fclose(fp);
    return 0;

}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
