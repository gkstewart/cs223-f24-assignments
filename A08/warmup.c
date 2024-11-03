#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pidB, pidC;

    printf("[%d] A0\n", getpid()); 

   
    pidB = fork();
    if (pidB < 0) {
        perror("this fork failed :(");
        return 1;
    } else if (pidB == 0) {  
        printf("[%d] B0\n", getpid()); 

        pid_t pidB1 = fork();
        if (pidB1 == 0) {
            printf("[%d] B1\n", getpid());
            printf("[%d] Bye\n", getpid());
        } else if (pidB1 > 0) {
            wait(NULL);  
            printf("[%d] Bye\n", getpid()); 
        }
    } else {  
        wait(NULL); 

        
        pidC = fork();
        if (pidC == 0) { 
            printf("[%d] C0\n", getpid()); 

            pid_t pidC1 = fork();
            if (pidC1 == 0) { 
                printf("[%d] C1\n", getpid());
                printf("[%d] Bye\n", getpid());
            } else if (pidC1 > 0) {
                wait(NULL);  
                printf("[%d] Bye\n", getpid()); 
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}
