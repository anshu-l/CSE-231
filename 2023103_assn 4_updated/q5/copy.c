#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Galat argument daal diye ho.\n");
        return 1;
    }

    char *source_file_ka_naam = argv[1];
    char *destination_directory_ka_naam = argv[2];

    FILE *source_file = fopen(source_file_ka_naam, "rb");
    if (source_file == NULL) {
        perror("Source file kholne mein dikkat hai");
        return 1;
    }

    struct stat directory_stat;
    if (stat(destination_directory_ka_naam, &directory_stat) != 0 || !S_ISDIR(directory_stat.st_mode)) {
        printf("Destination valid nahi hai\n");
        fclose(source_file);
        return 1;
    }

    char destination_file_path[1024];
    snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s", destination_directory_ka_naam, source_file_ka_naam);

    FILE *destination_file = fopen(destination_file_path, "wb");
    if (destination_file == NULL) {
        perror("Destination file kholne mein dikkat hai");
        fclose(source_file);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
        fwrite(buffer, 1, bytes_read, destination_file);
    }

    fclose(source_file);
    fclose(destination_file);

    printf("%s ko %s mein copy kar diya gaya hai\n", source_file_ka_naam, destination_directory_ka_naam);

    return 0;
}
