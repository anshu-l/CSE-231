#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define MAX_LINE_LENGTH 1024
#define MAX_STATIONS 100000

typedef struct {
    char station_name[256];
    double min_temp;
    double max_temp;
    double total_temp;
    int count;
} WeatherStation;

WeatherStation stations[MAX_STATIONS];
int station_count = 0;

// Find the index of a station in the array
int find_station_index(const char *name) {
    for (int i = 0; i < station_count; i++) {
        if (strcmp(stations[i].station_name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Add a new station or update an existing one
void add_or_update_station(const char *name, double temp) {
    int index = find_station_index(name);
    if (index == -1) {
        if (station_count >= MAX_STATIONS) {
            fprintf(stderr, "Error: Too many stations. Increase MAX_STATIONS.\n");
            exit(EXIT_FAILURE);
        }
        strcpy(stations[station_count].station_name, name);
        stations[station_count].min_temp = temp;
        stations[station_count].max_temp = temp;
        stations[station_count].total_temp = temp;
        stations[station_count].count = 1;
        station_count++;
    } else {
        if (temp < stations[index].min_temp) {
            stations[index].min_temp = temp;
        }
        if (temp > stations[index].max_temp) {
            stations[index].max_temp = temp;
        }
        stations[index].total_temp += temp;
        stations[index].count++;
    }
}

// Process file using mmap
void process_file_mmap(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Failed to get file stats");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (sb.st_size == 0) {
        fprintf(stderr, "Error: File is empty.\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char *file_content = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_content == MAP_FAILED) {
        perror("Failed to mmap file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    madvise(file_content, sb.st_size, MADV_SEQUENTIAL);

    char *line = file_content;
    char *file_end = file_content + sb.st_size;

    while (line < file_end) {
        char *next_line = memchr(line, '\n', file_end - line);  // Find the next newline
        if (!next_line) {
            next_line = file_end;  // Handle the last line without a newline
        }

        size_t line_length = next_line - line;

        if (line_length >= MAX_LINE_LENGTH) {
            fprintf(stderr, "Error: Line too long (truncated).\n");
            line = next_line + 1;  // Skip this line and move to the next
            continue;
        }

        char temp_line[MAX_LINE_LENGTH];
        strncpy(temp_line, line, line_length);
        temp_line[line_length] = '\0';  // Null-terminate the string

        char *station_name = strtok(temp_line, ";");
        char *temperature_str = strtok(NULL, ";");
        if (station_name && temperature_str) {
            double temp = atof(temperature_str);
            add_or_update_station(station_name, temp);
        }

        line = next_line + 1;  // Move to the next line
    }

    if (munmap(file_content, sb.st_size) == -1) {
        perror("Failed to munmap");
    }

    close(fd);
}

// Process file using fread
void process_file_fread(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char *station_name = strtok(line, ";");
        char *temperature_str = strtok(NULL, ";");
        if (station_name && temperature_str) {
            double temp = atof(temperature_str);
            add_or_update_station(station_name, temp);
        }
    }

    fclose(file);
}

// Print the results
void print_results() {
    for (int i = 0; i < station_count; i++) {
        double mean_temp = stations[i].total_temp / stations[i].count;
        printf("%s min=%.2f mean=%.2f max=%.2f\n",
               stations[i].station_name,
               stations[i].min_temp,
               mean_temp,
               stations[i].max_temp);
    }
}

int main() {
    const char *filename = "q1-10mil.txt";

    clock_t start_time, end_time;

    // Process using fread
    printf("Processing file using fread...\n");
    start_time = clock();
    process_file_fread(filename);
    end_time = clock();
    printf("Time taken using fread: %.2f seconds\n",
           (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Print results after fread
    print_results();

    // Reset station data for mmap
    station_count = 0;
    memset(stations, 0, sizeof(stations));

    // Process using mmap
    printf("\nProcessing file using mmap...\n");
    start_time = clock();
    process_file_mmap(filename);
    end_time = clock();
    printf("Time taken using mmap: %.2f seconds\n",
           (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Print results after mmap
    print_results();

    return 0;
}