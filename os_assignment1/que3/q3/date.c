#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void display_default_time() {
    time_t current_time;
    struct tm *local_tm;
    char time_buffer[100];

    time(&current_time);
    local_tm = localtime(&current_time);

    strftime(time_buffer, sizeof(time_buffer), "%c", local_tm);
    printf("Default time format: %s\n", time_buffer);
}

void display_utc_time() {
    time_t current_time;
    struct tm *utc_tm;
    char time_buffer[100];

    time(&current_time);
    utc_tm = gmtime(&current_time);

    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S UTC", utc_tm);
    printf("UTC time format: %s\n", time_buffer);
}

void display_rfc2822_time() {
    time_t current_time;
    struct tm *local_tm;
    char time_buffer[100];

    time(&current_time);
    local_tm = localtime(&current_time);

    strftime(time_buffer, sizeof(time_buffer), "%a, %d %b %Y %H:%M:%S %z", local_tm);
    printf("RFC 2822 time format: %s\n", time_buffer);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "-u") == 0) {
            display_utc_time();
        } else if (strcmp(argv[1], "-r") == 0) {
            display_rfc2822_time();
        } else {
            printf("Error: Unknown option %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    } else {
        display_default_time();
    }

    return EXIT_SUCCESS;
}

