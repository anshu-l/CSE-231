#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv) {
    char *path_ka_naam;
    struct dirent *pata_nhi_kya_naam_du;
    DIR *dir;

    if (argc == 1) {
        path_ka_naam = ".";
    }

    else {
        path_ka_naam = argv[1];
    }


    dir = opendir(path_ka_naam);

    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while((pata_nhi_kya_naam_du = readdir(dir)) != NULL ) {
        printf("%s\n", pata_nhi_kya_naam_du->d_name);
    }

    closedir(dir);
    return 0;
}