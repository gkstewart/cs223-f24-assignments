#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include "read_ppm.h"
#include "write_ppm.h"

#define NUM_THREADS 4

struct thread_data {
  int thread_id;
  int start_row;
  int end_row;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  struct ppm_pixel* pixels;
};

void* compute_mandelbrot(void* arg) {
  struct thread_data *data = (struct thread_data *)arg;
  int id = data->thread_id;
  int start_row = data->start_row;
  int end_row = data->end_row;
  int size = data->size;
  float xmin = data->xmin;
  float xmax = data->xmax;
  float ymin = data->ymin;
  float ymax = data->ymax;
  int maxIterations = data->maxIterations;
  struct ppm_pixel* pixels = data->pixels;

  for (int row = start_row; row < end_row; row++) {
    for (int col = 0; col < size; col++) {
      float x0 = xmin + (xmax - xmin) * col / size;
      float y0 = ymin + (ymax - ymin) * row / size;
      float x = 0, y = 0;
      int iteration = 0;
      while (x*x + y*y <= 4 && iteration < maxIterations) {
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iteration++;
      }
      struct ppm_pixel color;
      if (iteration < maxIterations) {
        if (iteration % 2 == 0) {
          color.red = 0;
          color.green = 128; 
          color.blue = 0; 
        } else {
          color.red = 255; 
          color.green = 105; 
          color.blue = 180; 
        }
      } else {
        color.red = 0;
        color.green = 0;
        color.blue = 0; 
      }
      pixels[row * size + col] = color;
    }
  }

  printf("Thread %d) sub-image block: cols (0, %d) to rows (%d, %d)\n", id, size, start_row, end_row);
  printf("Thread %d) finished\n", id);
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': 
        printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]);
        return 1;
    }
  }

  struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
  pthread_t threads[NUM_THREADS];
  struct thread_data tdata[NUM_THREADS];
  int rows_per_thread = size / 2;

  for (int i = 0; i < NUM_THREADS; i++) {
    tdata[i].thread_id = i;
    tdata[i].start_row = (i / 2) * rows_per_thread;
    tdata[i].end_row = (i / 2 + 1) * rows_per_thread;
    tdata[i].size = size;
    tdata[i].xmin = xmin;
    tdata[i].xmax = xmax;
    tdata[i].ymin = ymin;
    tdata[i].ymax = ymax;
    tdata[i].maxIterations = maxIterations;
    tdata[i].pixels = pixels;
    pthread_create(&threads[i], NULL, compute_mandelbrot, &tdata[i]);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  char filename[100];
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(NULL));
  write_ppm(filename, pixels, size, size);
  free(pixels);

  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, (double)clock() / CLOCKS_PER_SEC);
  printf("Writing file: %s\n", filename);

  return 0;
}
