#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t lock;      
int *line_counts;            

typedef struct {
    int thread_id;
    int start_index;
    int end_index;
    char **files;
    char *keyword;
} thread_args_t;

void *grep_thread(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    FILE *file;
    char line[1024];

    for (int i = args->start_index; i < args->end_index; i++) {
        file = fopen(args->files[i], "r");
        if (!file) {
            pthread_mutex_lock(&lock);
            fprintf(stderr, "Thread %d: Error opening file %s\n", args->thread_id, args->files[i]);
            pthread_mutex_unlock(&lock);
            continue;
        }

        int line_count = 0;
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, args->keyword)) {
                pthread_mutex_lock(&lock);
                printf("Thread %d) %s: %s", args->thread_id, args->files[i], line);
                pthread_mutex_unlock(&lock);
                line_count++;
            }
        }
        fclose(file);

        pthread_mutex_lock(&lock);
        line_counts[args->thread_id] += line_count;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <num_threads> <keyword> <file1> [file2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);
    char *keyword = argv[2];
    char **files = &argv[3];
    int num_files = argc - 3;

    if (num_threads <= 0) {
        fprintf(stderr, "Error: Number of threads must be greater than 0\n");
        return EXIT_FAILURE;
    }

    pthread_mutex_init(&lock, NULL);

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    thread_args_t *args = malloc(num_threads * sizeof(thread_args_t));
    line_counts = calloc(num_threads, sizeof(int)); 

    if (!threads || !args || !line_counts) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        pthread_mutex_destroy(&lock);
        free(threads);
        free(args);
        free(line_counts);
        return EXIT_FAILURE;
    }

    int files_per_thread = num_files / num_threads;
    int extra_files = num_files % num_threads;

    int current_file = 0;
    for (int i = 0; i < num_threads; i++) {
        args[i].thread_id = i;
        args[i].start_index = current_file;
        args[i].end_index = current_file + files_per_thread + (i < extra_files ? 1 : 0);
        args[i].files = files;
        args[i].keyword = keyword;
        current_file = args[i].end_index;

        if (pthread_create(&threads[i], NULL, grep_thread, &args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            pthread_mutex_destroy(&lock);
            free(threads);
            free(args);
            free(line_counts);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int total_lines = 0;
    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d found %d lines containing the keyword: %s\n", i, line_counts[i], keyword);
        total_lines += line_counts[i];
    }
    printf("Total lines found: %d\n", total_lines);

    pthread_mutex_destroy(&lock);
    free(threads);
    free(args);
    free(line_counts);

    return EXIT_SUCCESS;
}
