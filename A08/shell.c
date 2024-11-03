#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>

#define BUFFER_SIZE 1024

// color codes
#define ANSI_COLOR_PINK "\x1b[35m"  // pink!!!!!!!!!
#define ANSI_COLOR_RESET "\x1b[0m"

// get the prompt string with current directory
char* get_prompt() {
    char cwd[PATH_MAX];
    struct passwd *pw = getpwuid(geteuid());
    
    if (getcwd(cwd, sizeof(cwd)) != NULL && pw != NULL) {
        char *prompt = malloc(PATH_MAX + 50);
        snprintf(prompt, PATH_MAX + 50, ANSI_COLOR_PINK "%s@myshell: %s $ " ANSI_COLOR_RESET, pw->pw_name, cwd);
        return prompt;
    } else {
        perror("getcwd() error");
        return "myshell$ ";
    }
}

int main() {
    char input[BUFFER_SIZE];
    char *args[100];
    
    while (1) {
        // display prompt and read input
        char *prompt = get_prompt();
        printf("%s", prompt);
        free(prompt);

        // read input
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;

        // check for "exit" command
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // input into command and arguments
        int i = 0;
        char *token = strtok(input, " ");
        while (token != NULL && i < 99) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
        } else if (pid == 0) {
            if (execvp(args[0], args) < 0) {
                perror("command failed :(");
            }
            exit(1);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("command exited with status %d\n", WEXITSTATUS(status));
            } else {
                printf("command terminated abnormally\n");
            }
        }
    }

    printf("exiting shell.\n");
    return 0;
}
