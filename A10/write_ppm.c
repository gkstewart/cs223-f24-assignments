#include "write_ppm.h"
#include <stdio.h>
#include <stdlib.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file for writing");
        return;
    }

    fprintf(fp, "P6\n%d %d\n255\n", w, h);
    fwrite(pixels, sizeof(struct ppm_pixel), w * h, fp);
    fclose(fp);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pxs, int w, int h) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file for writing");
        return;
    }

    fprintf(fp, "P6\n%d %d\n255\n", w, h);
    for (int i = 0; i < h; i++) {
        fwrite(pxs[i], sizeof(struct ppm_pixel), w, fp);
    }
    fclose(fp);
}
