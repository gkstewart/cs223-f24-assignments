#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
    int size;              
    int used;              
    struct chunk *next;    
};

struct chunk *flist = NULL;

struct chunk *find_free_chunk(size_t size) {
    struct chunk *current = flist;
    struct chunk *previous = NULL;

    while (current) {
        if (current->size >= size) {
            if (previous) {
                previous->next = current->next;
            } else {
                flist = current->next;
            }
            return current;
        }
        previous = current;
        current = current->next;
    }

    return NULL;
}

void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    struct chunk *allocated = find_free_chunk(size);
    if (!allocated) {
        size_t total_size = size + sizeof(struct chunk);
        allocated = sbrk(total_size);
        if (allocated == (void *)-1) {
            return NULL;
        }
        allocated->size = size;
    }

    allocated->used = size;
    allocated->next = NULL;
    return (void *)(allocated + 1); 
}

void free(void *memory) {
    if (!memory) {
        return;
    }

    struct chunk *to_free = (struct chunk *)memory - 1;
    to_free->used = 0;

    to_free->next = flist;
    flist = to_free;
}
