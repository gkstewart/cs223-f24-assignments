#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': 
        printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); 
        return 1;
    }
  }

  printf("Generating Mandelbrot Set with size %dx%d\n", size, size);
  printf("X range = [%.4f, %.4f]\n", xmin, xmax);
  printf("Y range = [%.4f, %.4f]\n", ymin, ymax);

  struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
  if (pixels == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    return 1;
  }

  struct ppm_pixel* palette = malloc(maxIterations * sizeof(struct ppm_pixel));
  if (palette == NULL) {
    fprintf(stderr, "Error allocating memory for palette\n");
    free(pixels);
    return 1;
  }

  srand(time(0));
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 256;
    palette[i].green = rand() % 256;
    palette[i].blue = rand() % 256;
  }

  struct timeval start, end;
  gettimeofday(&start, NULL); 

  for (int row = 0; row < size; row++) {
    for (int col = 0; col < size; col++) {
      float x0 = xmin + (xmax - xmin) * col / (size - 1);
      float y0 = ymin + (ymax - ymin) * row / (size - 1);

      float x = 0, y = 0;
      int iteration = 0;
      while (x*x + y*y <= 4 && iteration < maxIterations) {
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iteration++;
      }

      pixels[row * size + col] = (iteration < maxIterations) ? palette[iteration] : (struct ppm_pixel){0, 0, 0};
    }
  }

  gettimeofday(&end, NULL); 
  double time_taken = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

  printf("Computed Mandelbrot set (%dx%d) in %f milliseconds\n", size, size, time_taken);

  char filename[50];
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));

  if (write_ppm(filename, pixels, size, size) == 0) {
    printf("Wrote image file %s\n", filename);
  } else {
    printf("Failed to write image file\n");
  }

  free(pixels);
  free(palette);

  return 0;
}
