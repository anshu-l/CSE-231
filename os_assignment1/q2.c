#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void binarySearch(int arr[], int left, int right, int target) {
    if (left > right) {
        return;
    }

    int mid = left + (right - left) / 2;

    if (arr[mid] == target) {
        printf("Element found at index %d\n", mid);
        exit(0);
    }

    if (arr[mid] > target) {
       
        int rc = fork();
        if (rc < 0) {
            perror("fork failed");
            exit(1);
        } else if (rc == 0) {
            binarySearch(arr, left, mid - 1, target);
            exit(1); 
        } else {
            wait(NULL);
        }
    } else {
        // Search the right half
        int rc = fork();
        if (rc < 0) {
            perror("fork failed");
            exit(1);
        } else if (rc == 0) {
            binarySearch(arr, mid + 1, right, target);
            exit(1); 
        } else {
            wait(NULL); 
        }
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int size = sizeof(arr) / sizeof(arr[0]);
    int target;

  
    printf("Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

 
    printf("Enter a target value: ");
    scanf("%d", &target);

   
    binarySearch(arr, 0, size - 1, target);

    printf("Element is not present in the array\n");

    return 0;
}
