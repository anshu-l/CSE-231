#include <stdio.h>

int main() {
    int n;
    int quantum;
    int i;
    int j;
    int time = 0;
    int  remaining;
    float total_turnaround = 0, total_response = 0;

    // Ask for the number of processes (must be 4 or more)
    do {
        printf("Enter number of processes (n=>4): ");
        scanf("%d", &n);
    } while (n < 4);

    int pid[n], arrival[n], burst[n], remaining_time[n], start[n], finish[n], turnaround[n], response[n];
    int first_execution[n]; 


    for (i = 0; i < n; i++) {
        printf("\nEnter Process ID for Process %d: ", i + 1);
        scanf("%d", &pid[i]);
        printf("Enter Arrival Time for Process %d: ", i + 1);
        scanf("%d", &arrival[i]);
        printf("Enter Burst Time for Process %d: ", i + 1);
        scanf("%d", &burst[i]);

        remaining_time[i] = burst[i];  
        first_execution[i] = 0;        
    }


    printf("\nEnter the time quantum: ");
    scanf("%d", &quantum);

    remaining = n; 
    while (remaining > 0) {
        for (i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && arrival[i] <= time) {
                if (first_execution[i] == 0) {

                    response[i] = time - arrival[i];
                    total_response += response[i];
                    first_execution[i] = 1; 
                }

                if (remaining_time[i] <= quantum) {

                    time += remaining_time[i];
                    remaining_time[i] = 0;
                    finish[i] = time;
                    turnaround[i] = finish[i] - arrival[i];
                    total_turnaround += turnaround[i];
                    remaining--;
                } else {

                    time += quantum;
                    remaining_time[i] -= quantum;
                }
            }
        }
    }

    printf("\nProcess Execution Order and Details:\n");
    printf("PID\tArrival\tBurst\tFinish\tTurnaround\tResponse\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t\t%d\n", pid[i], arrival[i], burst[i], finish[i], turnaround[i], response[i]);
    }


    printf("\nAverage Turnaround Time: %.2f", total_turnaround / n);
    printf("\nAverage Response Time: %.2f\n", total_response / n);

    return 0;
}

