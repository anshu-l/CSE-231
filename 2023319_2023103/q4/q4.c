#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Pajamaa input na diya karo\n");
        exit(1);
    }

    if (strcmp(argv[1], "-p1") == 0) {
        int file_descriptor = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_descriptor == -1) {
            perror("File kholne mein big big skill izzuez");
            exit(1);
        }

        if (dup2(file_descriptor, STDOUT_FILENO) == -1) {
            perror("dup2 is skill issued");
            close(file_descriptor);
            exit(1);
        }

        close(file_descriptor);

        printf("Yeh text file mein likha jaayega: %s\n", argv[2]);
        printf("Random text: %d, %s\n", 1234, "Tum toh pajama insaan ho");

    } else if (strcmp(argv[1], "-p2") == 0) {
        if (argc != 4) {
            printf("Fir pajama input daal diye ho\n");
            exit(1);
        }

        char *file_ka_naam = argv[3];
        int file_descriptor = open(file_ka_naam, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_descriptor == -1) {
            perror("File kholne mein dikkat hai");
            exit(1);
        }

        if (dup2(file_descriptor, STDOUT_FILENO) == -1) {
            perror("stdout ko file mein redirect karte waqt dikkat aayi");
            close(file_descriptor);
            exit(1);
        }

        if (dup2(file_descriptor, STDERR_FILENO) == -1) {
            perror("stderr ko file mein redirect karte waqt dikkat aayi");
            close(file_descriptor);
            exit(1);
        }

        close(file_descriptor);

        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork mein dikkat aayi");
            exit(1);
        } else if (pid == 0) {
            char *args[] = {"/bin/bash", "-c", argv[2], NULL};
            execvp(args[0], args);
            perror("execvp mein dikkat aayi");
            exit(1);
        } else {
            wait(NULL);
        }

    } else {
        printf("Pajama ho tum\n");
        exit(1);
    }

    return 0;
}
