#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ARRAY_SIZE 16  // Changed the macro name

// Function to merge two halves of the array
void combine(int array[], int left, int mid, int right) {
    int sizeLeft = mid - left + 1;
    int sizeRight = right - mid;

    int leftArray[sizeLeft], rightArray[sizeRight];

    // Copy data into temporary arrays
    for (int i = 0; i < sizeLeft; i++)
        leftArray[i] = array[left + i];
    for (int j = 0; j < sizeRight; j++)
        rightArray[j] = array[mid + 1 + j];

    // Merging the two halves
    int i = 0, j = 0, k = left;
    while (i < sizeLeft && j < sizeRight) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            i++;
        } else {
            array[k] = rightArray[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of leftArray
    while (i < sizeLeft) {
        array[k] = leftArray[i];
        i++;
        k++;
    }

    // Copy remaining elements of rightArray
    while (j < sizeRight) {
        array[k] = rightArray[j];
        j++;
        k++;
    }
}

// Modified merge sort function that works with child processes
void parallelMergeSort(int array[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Create pipes for inter-process communication
        int pipeLeft[2], pipeRight[2];
        pipe(pipeLeft);
        pipe(pipeRight);

        // Create first child process to sort left half
        pid_t leftChild = fork();

        if (leftChild == 0) {
            close(pipeLeft[0]);  // Close reading end in child
            parallelMergeSort(array, left, mid);  // Recursive sort on left half
            write(pipeLeft[1], array + left, (mid - left + 1) * sizeof(int));  // Send sorted data
            close(pipeLeft[1]);  // Close writing end
            exit(0);  // Child process exits
        } else {
            // Create second child process to sort right half
            pid_t rightChild = fork();

            if (rightChild == 0) {
                close(pipeRight[0]);  // Close reading end in child
                parallelMergeSort(array, mid + 1, right);  // Recursive sort on right half
                write(pipeRight[1], array + mid + 1, (right - mid) * sizeof(int));  // Send sorted data
                close(pipeRight[1]);  // Close writing end
                exit(0);  // Child process exits
            } else {
                // Parent waits for both children to finish
                wait(NULL);
                wait(NULL);

                // Read sorted left and right parts from pipes
                close(pipeLeft[1]);
                close(pipeRight[1]);
                read(pipeLeft[0], array + left, (mid - left + 1) * sizeof(int));
                read(pipeRight[0], array + mid + 1, (right - mid) * sizeof(int));
                close(pipeLeft[0]);
                close(pipeRight[0]);

                // Merge the sorted halves
                combine(array, left, mid, right);
            }
        }
    }
}

int main() {
    // Define a random unsorted array of 16 elements
    int dataArray[ARRAY_SIZE] = {5,9,8,7,6,1,2,3,4,12,13,14,15,16,10,11};

    // Print the unsorted array
    printf("Unsorted array:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", dataArray[i]);
    }
    printf("\n");

    // Perform parallel merge sort
    parallelMergeSort(dataArray, 0, ARRAY_SIZE - 1);

    // Print the sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", dataArray[i]);
    }
    printf("\n");

    return 0;
}
