#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h> 

#define RPM 72
#define SECTORS_PER_TRACK 100
#define TRACK_CHANGE_SEEK_TIME 2 
#define SECTOR_LATENCY (1000.0 / (RPM * SECTORS_PER_TRACK)) 

int abs_diff(int a, int b) {
    return abs(a - b);
}

void sstf(int head_position, int requests[], int n) {
    int visited[n];
    for (int i = 0; i < n; i++) visited[i] = 0;

    int total_seek_time = 0;
    int current_position = head_position;

    printf("\nInitial Head Position: %d\n\n", current_position);

    for (int i = 0; i < n; i++) {
        int closest_index = -1;
        int min_distance = INT_MAX;

        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int distance = abs_diff(current_position, requests[j]);
                if (distance < min_distance) {
                    min_distance = distance;
                    closest_index = j;
                }
            }
        }

        if (closest_index == -1) break;

    
        int distance = abs_diff(current_position, requests[closest_index]);
        int tracks_moved = distance / SECTORS_PER_TRACK;
        int seek_time = (distance >= SECTORS_PER_TRACK) ? tracks_moved * TRACK_CHANGE_SEEK_TIME : 0;
        int sectors_to_rotate = distance % SECTORS_PER_TRACK;
        double rotational_latency = sectors_to_rotate * SECTOR_LATENCY;

        total_seek_time += seek_time;

        visited[closest_index] = 1;

        printf("Request %d: %d\n", i + 1, requests[closest_index]);
        printf("Seek Time %d: %d ms\n", i + 1, seek_time);
        printf("Rotational Latency %d: %.2f ms\n\n", i + 1, rotational_latency);

        current_position = requests[closest_index];
    }

    printf("Total Seek Time: %d ms\n", total_seek_time);
}

int main() {
    int n;
    int head_position;

    printf("Enter the initial head position: ");
    scanf("%d", &head_position);

    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    int *requests = (int *)malloc(n * sizeof(int));
    if (requests == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter the disk requests (sectors):\n");
    for (int i = 0; i < n; i++) {
        printf("Request %d: ", i + 1);
        scanf("%d", &requests[i]);
    }

    sstf(head_position, requests, n);

    free(requests);

    return 0;
}