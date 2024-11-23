#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_DEPENDENCIES 100
#define MAX_LINE_LENGTH 1024

pthread_mutex_t tree_lock;
struct tree_node* root = NULL; // Root 

typedef struct {
    int thread_id;
    int start_index;
    int end_index;
    char **files;
} thread_args_t;

void process_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    char dependencies[MAX_DEPENDENCIES][64];
    int dep_count = 0;

    while (fgets(line, sizeof(line), file)) {
        char* include_ptr = strstr(line, "#include");
        if (include_ptr) {
            char* start = strchr(include_ptr, '\"'); // Find opening quote
            char* end = start ? strchr(start + 1, '\"') : NULL; // Find closing quote
            if (start && end && (end - start - 1 < 64)) {
                strncpy(dependencies[dep_count], start + 1, end - start - 1);
                dependencies[dep_count][end - start - 1] = '\0'; // Null-terminate
                dep_count++;
            }
        }
    }
    fclose(file);

    pthread_mutex_lock(&tree_lock);
    struct tree_node* file_node = insert(filename, root);
    if (root == NULL) {
        root = file_node; // Set the root if it's the first node
    }
    for (int i = 0; i < dep_count; i++) {
        insert(dependencies[i], root);
    }
    pthread_mutex_unlock(&tree_lock);
}

void* thread_function(void* arg) {
    thread_args_t* args = (thread_args_t*)arg;
    for (int i = args->start_index; i < args->end_index; i++) {
        process_file(args->files[i]);
    }
    return NULL;
}

void interactive_prompt() {
    char command[MAX_LINE_LENGTH];
    while (1) {
        printf("> ");
        if (!fgets(command, sizeof(command), stdin)) {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "quit") == 0) {
            break;
        } else if (strcmp(command, "list") == 0) {
            printSorted(root); // Print all files in alphabetical order
        } else {
            struct tree_node* node = find(command, root);
            if (node) {
                printf("%s has the following dependencies:\n", command);
            } else {
                printf("%s not found\n", command);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <num_threads> <file1> [file2 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);
    char** files = &argv[2];
    int num_files = argc - 2;

    if (num_threads <= 0 || num_files <= 0) {
        fprintf(stderr, "Error: Invalid number of threads or files\n");
        return EXIT_FAILURE;
    }

    pthread_t threads[num_threads];
    thread_args_t args[num_threads];
    pthread_mutex_init(&tree_lock, NULL);

    int files_per_thread = num_files / num_threads;
    int extra_files = num_files % num_threads;
    int current_file = 0;

    for (int i = 0; i < num_threads; i++) {
        args[i].thread_id = i;
        args[i].start_index = current_file;
        args[i].end_index = current_file + files_per_thread + (i < extra_files ? 1 : 0);
        args[i].files = files;
        current_file = args[i].end_index;

        if (pthread_create(&threads[i], NULL, thread_function, &args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Processing complete.\n");

    interactive_prompt();

    pthread_mutex_destroy(&tree_lock);
    clear(root);
    return EXIT_SUCCESS;
}
