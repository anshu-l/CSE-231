#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pehla_baccha, doosra_baccha, teesra_baccha, choutha_baccha;

    pehla_baccha = fork();
    if (pehla_baccha == 0) {
        printf("Pehla Baccha: 'list' program ko run kar raha hai...\n");
        execlp("./list", "list", NULL);
        perror("exec mein kuch galat hai");
        exit(1);
    }

    doosra_baccha = fork();
    if (doosra_baccha == 0) {
        printf("Doosra Baccha: 'countw' program ko run kar raha hai...\n");
        execlp("./countw", "countw", "Makefile", NULL);
        perror("exec mein kuch galat hai");
        exit(1);
    }

    teesra_baccha = fork();
    if (teesra_baccha == 0) {
        printf("Teesra Baccha: 'copy' program ko run kar raha hai...\n");
        execlp("./copy", "copy", "list.c", "../", NULL);
        perror("exec mein kuch galat hai");
        exit(1);
    }

    choutha_baccha = fork();
    if (choutha_baccha == 0) {
        printf("Choutha Baccha: 'move' program ko run kar raha hai...\n");
        execlp("./move", "move", "/mnt/c/Users/midam/OneDrive/Desktop/Sem3/Os/Assignment4/q5", "/mnt/c/Users/midam/OneDrive/Desktop/Sem3/Os/Assignment4/q5/q5_files", NULL);
        perror("exec mein kuch galat hai");
        exit(1);
    }

    waitpid(pehla_baccha, NULL, 0);
    waitpid(doosra_baccha, NULL, 0);
    waitpid(teesra_baccha, NULL, 0);
    waitpid(choutha_baccha, NULL, 0);

    printf("Maa-Baap: Sabhi bacche apne kaam poora kar chuke hain.\n");

    return 0;
}
