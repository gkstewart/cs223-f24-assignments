/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: September 25th, 2024
 * Description: This program allows for users to find the most dancable songs in the data set songslist.csv.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct song
struct Song {
    char title[32];
    char artist[32];
    int duration_ms;
    float danceability;
    float energy;
    float tempo;
    float valence;
    struct Song *next;
};

// create a new song node
struct Song* create_song(char *title, char *artist, int duration_ms, float danceability, float energy, float tempo, float valence) {
    struct Song *new_song = (struct Song*) malloc(sizeof(struct Song));
    if (new_song == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }
    strcpy(new_song->title, title);
    strcpy(new_song->artist, artist);
    new_song->duration_ms = duration_ms;
    new_song->danceability = danceability;
    new_song->energy = energy;
    new_song->tempo = tempo;
    new_song->valence = valence;
    new_song->next = NULL;
    return new_song;
}

// add a song to the linked list
void insert_song(struct Song **head, struct Song *new_song) {
    new_song->next = *head;
    *head = new_song;
}

// load songs from a CSV file into a linked list
void load_songs(struct Song **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    char line[256];
    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        char title[32], artist[32];
        int duration_ms;
        float danceability, energy, tempo, valence;
        sscanf(line, "%[^,],%[^,],%d,%f,%f,%f,%f", title, artist, &duration_ms, &danceability, &energy, &tempo, &valence);
        struct Song *new_song = create_song(title, artist, duration_ms, danceability, energy, tempo, valence);
        insert_song(head, new_song);
    }

    fclose(file);
}

// find and remove the most danceable song from the list
struct Song* find_and_remove_most_danceable(struct Song **head) {
    if (*head == NULL) return NULL;

    struct Song *max_dance_song = *head, *prev = NULL, *curr = *head, *prev_max = NULL;

    while (curr != NULL) {
        if (curr->danceability > max_dance_song->danceability) {
            max_dance_song = curr;
            prev_max = prev;
        }
        prev = curr;
        curr = curr->next;
    }

    // remove the most danceable song from the list
    if (prev_max == NULL) { // max_dance_song is now the head
        *head = (*head)->next;
    } else {
        prev_max->next = max_dance_song->next;
    }

    return max_dance_song;
}

// print a song
void print_song(struct Song *song) {
    if (song == NULL) return;
    int minutes = song->duration_ms / 60000;
    int seconds = (song->duration_ms % 60000) / 1000;
    printf("%s\t%s\t(%d:%02ds) D: %.3f E: %.3f T: %.3f V: %.3f\n", 
        song->title, song->artist, minutes, seconds, song->danceability, song->energy, song->tempo, song->valence);
}

// print the song list
void print_song_list(struct Song *head) {
    struct Song *curr = head;
    int index = 0;
    while (curr != NULL) {
        printf("%d) ", index++);
        print_song(curr);
        curr = curr->next;
    }
    printf("\n");
}

// free the linked list
void free_song_list(struct Song *head) {
    struct Song *curr;
    while (head != NULL) {
        curr = head;
        head = head->next;
        free(curr);
    }
}

// main function
int main() {
    struct Song *head = NULL;
    load_songs(&head, "songlist.csv");

    char choice;
    while (1) {
        printf("\n=======================\n");
        printf("press 'd' to show the most danceable song (any other key to quit): ");
        choice = getchar();
        getchar();  
        if (choice != 'd') break;

        struct Song *most_danceable = find_and_remove_most_danceable(&head);
        if (most_danceable == NULL) {
            printf("dataset contains 0 songs\n");
        } else {
            printf("\n---------------------------------------- Most danceable ------------------------------------\n");
            print_song(most_danceable);
            printf("--------------------------------------------------------------------------------------------\n");
            free(most_danceable);
        }

        printf("\n");
        print_song_list(head);
    }

    // free remaining songs before quitting
    free_song_list(head);

    return 0;
}
