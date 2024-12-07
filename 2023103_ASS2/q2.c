y#include <stdio.h>
#include <stdlib.h>

// Function to translate logical to physical address based on segment
int translateAddress(unsigned int logicalAddress) {
    unsigned int codeBase = 32768;    // 32KB
    unsigned int codeLimit = 2048;    // 2KB

    unsigned int heapBase = 34816;    // 34KB
    unsigned int heapLimit = 3072;    // 3KB

    unsigned int stackBase = 28672;   // 28KB
    unsigned int stackLimit = 2048;   // 2KB (grows downwards)

    // Extract the segment selector (top 2 bits)
    unsigned int segmentSelector = (logicalAddress >> 14) & 0x03;
    // Extract the offset (bottom 14 bits)
    unsigned int offset = logicalAddress & 0x3FFF;

    switch (segmentSelector) {
        // Case 00: Code Segment
        case 0x00:
            if (offset < codeLimit) {
                return codeBase + offset;  // Valid code segment access
            } else {
                printf("Segmentation Fault in Code Segment\n");
                return -1;  // Code segment limit exceeded
            }
        
        // Case 01: Heap Segment
        case 0x01:
            if (offset < heapLimit) {
                return heapBase + offset;  // Valid heap segment access
            } else {
                printf("Segmentation Fault in Heap Segment\n");
                return -1;  // Heap segment limit exceeded
            }

        // Case 10: Stack Segment (grows downward)
        case 0x11:  
            if (offset < stackLimit) {
                return stackBase - offset;  // Valid stack segment access
            } else {
                printf("Segmentation Fault in Stack Segment\n");
                return -1;  // Stack segment limit exceeded
            }

        // Invalid segment selector (outside the defined ranges)
        default:
            printf("Segmentation Fault: Invalid Segment\n");
            return -1;  // Invalid segment selected
    }
}

int main() {
    char logicalAddressInput[5];  // Input buffer for 16-bit hex address
    unsigned int logicalAddress;

    // User input
    printf("Enter a 16-bit logical address in hex (e.g., a56f): ");
    scanf("%s", logicalAddressInput);
    
    // Convert the input from hex to an integer
    char *endPtr;
    logicalAddress = strtol(logicalAddressInput, &endPtr, 16);
    
    if (*endPtr != '\0') {
        printf("Invalid hexadecimal input: %s\n", logicalAddressInput);
        return 1;  // Exit or handle error
    }
    
    // Translate logical address to physical address
    int physicalAddress = translateAddress(logicalAddress);
    
    if (physicalAddress != -1) {
        printf("Physical Address: %04X\n", physicalAddress);
    }

    return 0;
}