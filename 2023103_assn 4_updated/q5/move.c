#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Pajama insaan wale input naa diya karo.\n");
        return 1;
    }

    char *from_wali_directory = argv[1];
    char *destination_directory = argv[2];

    struct stat dest_stat;
    if (stat(destination_directory, &dest_stat) != 0 || !S_ISDIR(dest_stat.st_mode)) {
        printf("Destination ek valid directory nahi hai\n");
        return 1;
    }

    DIR *sauce_dir = opendir(from_wali_directory);
    if (sauce_dir == NULL) {
        perror("Source directory kholne mein dikkat hai");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(sauce_dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char source_path[1024], dest_path[1024];
        snprintf(source_path, sizeof(source_path), "%s/%s", from_wali_directory, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", destination_directory, entry->d_name);

        if (rename(source_path, dest_path) != 0) {
            perror("Move karne mein dikkat hai");
            closedir(sauce_dir);
            return 1;
        }
    }

    closedir(sauce_dir);
    printf("Kaam Complete, Chalo pasie nikaloo\n" );

    return 0;
}
