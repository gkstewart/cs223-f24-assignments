/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: September 17th, 2024
 * Description: This program reads songs from a text file into an array and then prints it out in a list.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// arrays to hold song information and variables
typedef struct {
    char title[128];   // Maximum length for the title
    char artist[128];  // Maximum length for the artist
    int duration_ms;
    float danceability;
    float energy;
    float tempo;
    float valence;
} Song;

// set up for later functions 
int count_songs(FILE *file);
void load_songs(FILE *file, Song *songs, int num_songs);
void print_songs(Song *songs, int num_songs);

int main() {
    FILE *file = fopen("songlist.csv", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // read songs from the first line of the file
    int num_songs = count_songs(file);
    if (num_songs <= 0) {
        fprintf(stderr, "No songs found in file.\n");
        fclose(file);
        return 1;
    }

    // allocate memory for the songs
    Song *songs = (Song *)malloc(num_songs * sizeof(Song));
    if (!songs) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    // load song data into the array
    load_songs(file, songs, num_songs);

    // print the songs
    print_songs(songs, num_songs);

    // free the memory and close the file
    free(songs);
    fclose(file);

    return 0;
}

// function to count the number of songs in the file
int count_songs(FILE *file) {
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);  // read the first line
    
    int num_songs = atoi(strtok(buffer, ","));
    
    
    fgets(buffer, sizeof(buffer), file);

    return num_songs;
}

// function to load song data from the file
void load_songs(FILE *file, Song *songs, int num_songs) {
    char buffer[256];
    for (int i = 0; i < num_songs; i++) {
        fgets(buffer, sizeof(buffer), file);

        // parse song data
        strcpy(songs[i].title, strtok(buffer, ","));
        strcpy(songs[i].artist, strtok(NULL, ","));
        songs[i].duration_ms = atoi(strtok(NULL, ","));
        songs[i].danceability = atof(strtok(NULL, ","));
        songs[i].energy = atof(strtok(NULL, ","));
        songs[i].tempo = atof(strtok(NULL, ","));
        songs[i].valence = atof(strtok(NULL, ","));
    }
}

// function to print the songs
void print_songs(Song *songs, int num_songs) {
    printf("Welcome to Dynamic Donna's Danceability Directory.\n\n");

    for (int i = 0; i < num_songs; i++) {
        int minutes = songs[i].duration_ms / 60000;
        int seconds = (songs[i].duration_ms % 60000) / 1000;
        printf("%d) %-30s artist: %-20s duration: %d:%02d D: %.3f E: %.3f T: %.3f V: %.3f\n", 
               i, songs[i].title, songs[i].artist, minutes, seconds, 
               songs[i].danceability, songs[i].energy, songs[i].tempo, songs[i].valence);
    }
}
