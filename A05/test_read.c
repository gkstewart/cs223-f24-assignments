/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: September 2024
 * Description: testfile for read_ppm
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
    int width, height;
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &width, &height);
    if (!pixels) {
        return 1;
    }

    printf("Testing file feep-raw.ppm: %d %d\n", width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            struct ppm_pixel p = pixels[i * width + j];
            printf("(%d,%d,%d) ", p.red, p.green, p.blue);
        }
        printf("\n");
    }

    free(pixels); // free the allocated memory
    return 0;
}

