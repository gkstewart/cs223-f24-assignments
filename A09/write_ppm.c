#include "write_ppm.h"
#include <stdio.h>
#include <stdlib.h>

int write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
  FILE *fp = fopen(filename, "wb"); // "wb" for writing in binary mode
  if (fp == NULL) {
    perror("Failed to open file for writing");
    return -1; // Return -1 on file open error
  }

  // Write the PPM header
  fprintf(fp, "P6\n%d %d\n255\n", w, h);

  // Write the pixel data
  for (int i = 0; i < w * h; i++) {
    if (fwrite(&pixels[i], sizeof(struct ppm_pixel), 1, fp) != 1) {
      fclose(fp);
      return -1; // Return -1 on write error
    }
  }

  fclose(fp);
  return 0; // Return 0 on success
}
