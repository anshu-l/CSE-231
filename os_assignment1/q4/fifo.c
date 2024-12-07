#include <stdio.h>

struct Process {
    int pid;       // Process ID
    int arrival;   // Arrival Time
    int burst;     // Burst Time
    int start;     // Start Time
    int finish;    // Finish Time
    int turn_around; // Turnaround Time
    int response;  // Response Time
};

int main() {
    int n, i, current_time = 0;
    float total_turnaround = 0, total_response = 0;


    do {
        printf("Enter number of processes (must be 4 or more): ");
        scanf("%d", &n);
    } while (n < 4);

    struct Process p[n];


    for (i = 0; i < n; i++) {
        printf("\nEnter Process ID for Process %d: ", i + 1);
        scanf("%d", &p[i].pid);
        printf("Enter Arrival Time for Process %d: ", i + 1);
        scanf("%d", &p[i].arrival);
        printf("Enter Burst Time for Process %d: ", i + 1);
        scanf("%d", &p[i].burst);
    }


    for (i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival > p[j].arrival) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }


    for (i = 0; i < n; i++) {
        if (current_time < p[i].arrival) {
            current_time = p[i].arrival;  
        }

        p[i].start = current_time;
        p[i].finish = p[i].start + p[i].burst;
        current_time = p[i].finish;

        p[i].turn_around = p[i].finish - p[i].arrival;
        total_turnaround += p[i].turn_around;

        p[i].response = p[i].start - p[i].arrival;
        total_response += p[i].response;
    }


    printf("\nProcess Execution Order and Details:\n");
    printf("PID      Arrival  Burst  TatTime  ResponseTime  \n");
    for (i = 0; i < n; i++) {
        printf("%d\t   %d\t    %d\t     %d\t    \t%d\n",
                    p[i].pid, p[i].arrival, p[i].burst, 
                     p[i].turn_around, p[i].response);
    }


    printf("\nAverage Turnaround Time: %.2f", total_turnaround / n);
    printf("\nAverage Response Time: %.2f\n", total_response / n);

    return 0;
}

