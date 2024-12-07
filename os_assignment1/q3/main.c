#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;
    int status;

    // Fork the first child process
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0) {
        // Inside the first child process
        execl("./date", "date","-r", (char *)NULL); // Execute date program
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    // Fork the second child process
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0) {
        // Inside the second child process
        execl("./cal", "cal","9","2024", (char *)NULL); // Execute cal program
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    // Fork the third child process
    pid3 = fork();
    if (pid3 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid3 == 0) {
        // Inside the third child process
        execl("./uptime", "uptime", (char *)NULL); // Execute uptime program
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    // Parent process waits for all child processes to finish
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    waitpid(pid3, &status, 0);

    printf("All child processes have completed.\n");

    return 0;
}

