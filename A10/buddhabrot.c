#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "read_ppm.h"
#include "write_ppm.h"

#define MAX_THREADS 4

typedef struct {
    int thread_id;
    int size;
    float xmin, xmax, ymin, ymax;
    int maxIterations;
    int **counts;
    int *maxCount;
    pthread_mutex_t *mutex;
    pthread_barrier_t *barrier;
    struct ppm_pixel **image;
} ThreadData;

void *compute_buddhabrot(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int size = data->size;
    float xmin = data->xmin, xmax = data->xmax;
    float ymin = data->ymin, ymax = data->ymax;
    int maxIterations = data->maxIterations;
    int thread_id = data->thread_id;
    int **counts = data->counts;
    int *maxCount = data->maxCount;
    pthread_mutex_t *mutex = data->mutex;

    int start_row = (thread_id / 2) * (size / 2);
    int end_row = start_row + (size / 2);
    int start_col = (thread_id % 2) * (size / 2);
    int end_col = start_col + (size / 2);

    printf("Thread %ld) sub-image block: cols (%d, %d) to rows (%d, %d)\n", pthread_self(), start_col, end_col, start_row, end_row);

    for (int row = start_row; row < end_row; row++) {
        for (int col = start_col; col < end_col; col++) {
            float xfrac = (float)col / size;
            float yfrac = (float)row / size;
            float x0 = xmin + xfrac * (xmax - xmin);
            float y0 = ymin + yfrac * (ymax - ymin);

            float x = 0, y = 0;
            int iter = 0;

            while (iter < maxIterations && x * x + y * y < 4.0) {
                float xtmp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtmp;
                iter++;
            }

            if (iter < maxIterations) { 
                x = 0;
                y = 0;
                while (x * x + y * y < 4.0) {
                    float xtmp = x * x - y * y + x0;
                    y = 2 * x * y + y0;
                    x = xtmp;

                    int yrow = (int)(size * (y - ymin) / (ymax - ymin));
                    int xcol = (int)(size * (x - xmin) / (xmax - xmin));
                    if (yrow >= 0 && yrow < size && xcol >= 0 && xcol < size) {
                        pthread_mutex_lock(mutex);
                        counts[yrow][xcol]++;
                        if (counts[yrow][xcol] > *maxCount) {
                            *maxCount = counts[yrow][xcol];
                        }
                        pthread_mutex_unlock(mutex);
                    }
                }
            }
        }
    }

    pthread_barrier_wait(data->barrier);

    for (int row = start_row; row < end_row; row++) {
        for (int col = start_col; col < end_col; col++) {
            float gamma = 0.681;
            float factor = 1.0 / gamma;
            if (counts[row][col] > 0) {
                float value = log((float)counts[row][col]) / log((float)(*maxCount));
                value = pow(value, factor);
                data->image[row][col].red = (unsigned char)(value * 255);
                data->image[row][col].green = (unsigned char)(value * 255);
                data->image[row][col].blue = (unsigned char)(value * 255);
            } else {
                data->image[row][col].red = 0;
                data->image[row][col].green = 0;
                data->image[row][col].blue = 0;
            }
        }
    }

    printf("Thread %ld) finished\n", pthread_self());
    return NULL;
}

int main(int argc, char *argv[]) {
    int size = 480;
    float xmin = -2.0, xmax = 0.47, ymin = -1.12, ymax = 1.12;
    int maxIterations = 1000, numThreads = MAX_THREADS;

    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case 'p': numThreads = atoi(optarg); break;
            default: fprintf(stderr, "Usage: %s -s <size> -l <xmin> -r <xmax> -t <ymax> -b <ymin> -p <numThreads>\n", argv[0]); return 1;
        }
    }

    printf("Generating Buddhabrot with size %dx%d\n", size, size);
    printf("  Num threads = %d\n", numThreads);
    printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
    printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

    pthread_t threads[MAX_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, numThreads);

    int **counts = calloc(size, sizeof(int *));
    struct ppm_pixel **image = calloc(size, sizeof(struct ppm_pixel *));
    for (int i = 0; i < size; i++) {
        counts[i] = calloc(size, sizeof(int));
        image[i] = calloc(size, sizeof(struct ppm_pixel));
    }

    int maxCount = 0;
    ThreadData threadData[MAX_THREADS];
    clock_t start = clock();
    for (int i = 0; i < numThreads; i++) {
        threadData[i] = (ThreadData){i, size, xmin, xmax, ymin, ymax, maxIterations, counts, &maxCount, &mutex, &barrier, image};
        pthread_create(&threads[i], NULL, compute_buddhabrot, &threadData[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Computed buddhabrot set (%dx%d) in %.6f seconds\n", size, size, elapsed);

    char filename[64];
    sprintf(filename, "buddhabrot-%d-%ld.ppm", size, time(NULL));
    printf("Writing file: %s\n", filename);
    write_ppm_2d(filename, image, size, size);

    for (int i = 0; i < size; i++) {
        free(counts[i]);
        free(image[i]);
    }
    free(counts);
    free(image);

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    return 0;
}
