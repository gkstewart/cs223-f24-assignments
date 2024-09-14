/*Main driver file for assignment two. This program fills an array with three or more songs and then allows the user to edit the data in the array.
 * Gabrielle Stewart
 * September 10th, 2024
 */

#include <stdio.h>
#include <string.h>

// make the struct to hold song information
typedef struct {
    char title[32];
    char artist[32];
    int duration_minutes;
    int duration_seconds;
    float danceability;
} Song;

// function prototypes
void print_songs(Song songs[], int count);
void edit_song(Song songs[], int count);

int main() {
    // initialize the array for the songs
    Song songs[3] = {
        {"After Midnight", "Chappell Roan", 3, 23, 0.75},
        {"Succubus", "Ken Carson", 2, 27, 0.97},
        {"Life Goes On", "The Sundays", 3, 59, 0.33}
    };

    int choice;

    printf("Welcome to Gabby's really awesome and cool song list.\n\n");

    // Print the list of songs
    print_songs(songs, 3);

    // Prompt user to enter a song ID to edit
    printf("=======================\n");
    printf("enter a song id to edit [0,1,2]: ");
    scanf("%d%*c", &choice);

    if (choice >= 0 && choice < 3) {
        edit_song(songs, 3);
        print_songs(songs, 3);
    } else {
        printf("invalid song ID.\n");
    }

    return 0;
}

// Function to print the list of songs
void print_songs(Song songs[], int count) {
    for (int i = 0; i < count; ++i) {
        printf("%d) %-20s artist: %-20s duration: %d:%02d  danceability: %.2f\n",
               i,
               songs[i].title,
               songs[i].artist,
               songs[i].duration_minutes,
               songs[i].duration_seconds,
               songs[i].danceability);
    }
}

// Function to edit a song
void edit_song(Song songs[], int count) {
    int id;
    char attribute[32];
    char buffer[32];
    
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    fgets(attribute, sizeof(attribute), stdin);
    // remove newline character manually
    attribute[strcspn(attribute, "\n")] = '\0';
    
    printf("Enter the song ID to edit: ");
    scanf("%d%*c", &id);

    if (id < 0 || id >= count) {
        printf("Invalid song ID.\n");
        return;
    }

    if (strcmp(attribute, "artist") == 0) {
        printf("Enter a new artist name: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline char
        strcpy(songs[id].artist, buffer);
    } else if (strcmp(attribute, "title") == 0) {
        printf("Enter a new title: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline character
        strcpy(songs[id].title, buffer);
    } else if (strcmp(attribute, "duration") == 0) {
        int minutes, seconds;
        printf("Enter a duration (minutes): ");
        scanf("%d", &minutes);
        printf("Enter a duration (seconds): ");
        scanf("%d%*c", &seconds);
        songs[id].duration_minutes = minutes;
        songs[id].duration_seconds = seconds;
    } else if (strcmp(attribute, "danceability") == 0) {
        float danceability;
        printf("Enter a new danceability (0.00 to 1.00): ");
        scanf("%f%*c", &danceability);
        songs[id].danceability = danceability;
    } else {
        printf("Invalid attribute.\n");
    }
}
