#include <stdio.h>

typedef struct {
    int pid;            
    int arrivalTime;      
    int burstTime;     
    int remainingTime;  
    int TaT;             
    int responseTime;    
} 
Process;


void getProcessDetails(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\nEnter process ID for process %d: ", i + 1);
        scanf("%d", &processes[i].pid);

        printf("Enter Arrival Time for process %d: ", i + 1);
        scanf("%d", &processes[i].arrivalTime);

        printf("Enter Burst Time for process %d: ", i + 1);
        scanf("%d", &processes[i].burstTime);
        
        processes[i].remainingTime = processes[i].burstTime;
    }
}


void findSRTF(Process processes[], int n) {
    int completed = 0, currentTime = 0, smallest = -1;
    int minRemaining = 9999999;
    int firstResponse[n]; 


    for (int i = 0; i < n; i++) {
        firstResponse[i] = -1; 
    }

    while (completed != n) {
        minRemaining = 9999999;
        smallest = -1;


        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime < minRemaining && processes[i].remainingTime > 0) {
                minRemaining = processes[i].remainingTime;
                smallest = i;
            }
        }

        if (smallest == -1) {
            currentTime++;
            continue;
        }


        if (firstResponse[smallest] == -1) {
            processes[smallest].responseTime = currentTime - processes[smallest].arrivalTime;
            firstResponse[smallest] = currentTime;
        }


        processes[smallest].remainingTime--;
        currentTime++;


        if (processes[smallest].remainingTime == 0) {
            completed++;
            processes[smallest].TaT = currentTime - processes[smallest].arrivalTime;
        }
    }
}


void displayProcessDetails(Process processes[], int n) {
    printf("\nProcess Details and SRTF Scheduling Results:\n");

    printf("PID\tArrival\tBurst\tTurnaround\tResponse\n");

    float avgTurnaround = 0, avgResponse = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\n", processes[i].pid, processes[i].arrivalTime, processes[i].burstTime,
               processes[i].TaT, processes[i].responseTime);

        avgTurnaround += processes[i].TaT;
        avgResponse += processes[i].responseTime;
    }

    printf("\nAverage Turnaround Time: %.2f", avgTurnaround / n);
    printf("\nAverage Response Time: %.2f\n", avgResponse / n);
}

int main() {
    int n;


    printf("Enter the number of processes: ");
    scanf("%d", &n);


    if (n < 4) {
        printf("Error: The number of processes should be 4 or greater.\n");
        return 1;
    }


    Process processes[n];


    getProcessDetails(processes, n);


    findSRTF(processes, n);


    displayProcessDetails(processes, n);

    return 0;
}

