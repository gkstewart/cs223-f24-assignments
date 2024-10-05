/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: September 2024
 * Description: test file for write_ppm
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main() {
    int width, height;
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &width, &height);
    if (!pixels) {
        return 1;
    }

    write_ppm("test.ppm", pixels, width, height);

    struct ppm_pixel* test_pixels = read_ppm("test.ppm", &width, &height);
    if (!test_pixels) {
        free(pixels);
        return 1;
    }

    printf("Testing file test.ppm: %d %d\n", width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            struct ppm_pixel p = test_pixels[i * width + j];
            printf("(%d,%d,%d) ", p.red, p.green, p.blue);
        }
        printf("\n");
    }

    free(pixels);
    free(test_pixels);
    return 0;

