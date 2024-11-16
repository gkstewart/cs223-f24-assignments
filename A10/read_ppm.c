#include "read_ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    char format[3];
    fscanf(fp, "%2s\n", format);
    if (strcmp(format, "P6") != 0) {
        fprintf(stderr, "Invalid PPM format\n");
        fclose(fp);
        return NULL;
    }

    fscanf(fp, "%d %d\n255\n", w, h);
    struct ppm_pixel* pixels = malloc(*w * *h * sizeof(struct ppm_pixel));
    fread(pixels, sizeof(struct ppm_pixel), *w * *h, fp);
    fclose(fp);
    return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
    struct ppm_pixel* flat_pixels = read_ppm(filename, w, h);
    if (!flat_pixels) return NULL;

    struct ppm_pixel** pixels = malloc(*h * sizeof(struct ppm_pixel*));
    for (int i = 0; i < *h; i++) {
        pixels[i] = &flat_pixels[i * (*w)];
    }
    return pixels;
}
