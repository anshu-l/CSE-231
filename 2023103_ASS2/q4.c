#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define PAGE_SIZE 4096           // 4KB page size
#define PD_ENTRIES 1024          // 2^10
#define PT_ENTRIES 1024          // 2^10
#define OFFSET_BITS 12           // 12 bits offset
#define PT_INDEX_BITS 10         // 10 bits for PT index
#define PD_INDEX_BITS 10         // 10 bits for PD index
#define MAX_LOGS 1000            // Maximum number of logs for events

// Structure for a page table entry (now 4 bytes)
typedef struct {
    uint32_t *page_frame;  // Points to physical memory page frame (4 bytes)
} PageTableEntry;

// Structure for a page directory entry
typedef struct {
    PageTableEntry *page_table;  // Points to a page table
} PageDirectoryEntry;

// Virtual memory structure
typedef struct {
    PageDirectoryEntry pd[PD_ENTRIES];  // Page directory with 1024 entries
    uint8_t *memory;                    // Simulated physical memory
    int page_hits;
    int page_misses;
    int page_faults;                    // Count of page faults
    char event_logs[MAX_LOGS][150];     // Unified log for hits, misses, and faults
    int event_log_count;
} VirtualMemory;

// Function to extract the offset from a virtual address
uint32_t get_offset(uint32_t va) {
    return va & 0xFFF;
}

// Function to extract the page table index from a virtual address
uint32_t get_pt_index(uint32_t va) {
    return (va >> OFFSET_BITS) & 0x3FF;
}

// Function to extract the page directory index from a virtual address
uint32_t get_pd_index(uint32_t va) {
    return (va >> (OFFSET_BITS + PT_INDEX_BITS)) & 0x3FF;
}

// Initialize virtual memory
void init_vm(VirtualMemory *vm, size_t memory_size) {
    for (int i = 0; i < PD_ENTRIES; i++) {
        vm->pd[i].page_table = NULL;  // Initialize all PD entries to NULL
    }
    vm->memory = (uint8_t *)malloc(memory_size);
    vm->page_hits = 0;
    vm->page_misses = 0;
    vm->page_faults = 0;
    vm->event_log_count = 0;
}

// Function to handle page fault and set up page table
void handle_page_fault(VirtualMemory *vm, uint32_t va) {
    uint32_t pd_index = get_pd_index(va);
    uint32_t pt_index = get_pt_index(va);

    vm->page_faults++;  // Increment the page fault counter

    // Allocate page table if not present
    if (vm->pd[pd_index].page_table == NULL) {
        vm->pd[pd_index].page_table = (PageTableEntry *)malloc(PT_ENTRIES * sizeof(PageTableEntry));
        for (int i = 0; i < PT_ENTRIES; i++) {
            vm->pd[pd_index].page_table[i].page_frame = NULL;  // Initialize all PT entries to NULL
        }
        snprintf(vm->event_logs[vm->event_log_count++], 150, "Page Fault: Allocated new page table for PD index %d", pd_index);
    }

    // Allocate page frame if not present
    if (vm->pd[pd_index].page_table[pt_index].page_frame == NULL) {
        vm->pd[pd_index].page_table[pt_index].page_frame = (uint32_t *)malloc(PAGE_SIZE);
        snprintf(vm->event_logs[vm->event_log_count++], 150, "Page Fault: Allocated new page frame for PT index %d in PD index %d", pt_index, pd_index);
    }
}

// Simulate loading data from a virtual address
uint8_t load(VirtualMemory *vm, uint32_t va) {
    uint32_t pd_index = get_pd_index(va);
    uint32_t pt_index = get_pt_index(va);
    uint32_t offset = get_offset(va);

    // Check if PD and PT entries are valid
    if (vm->pd[pd_index].page_table != NULL &&
        vm->pd[pd_index].page_table[pt_index].page_frame != NULL) {
        vm->page_hits++;
        snprintf(vm->event_logs[vm->event_log_count++], 150, "LOAD at VA: 0x%X | PD Index: %d, PT Index: %d, Offset: %d | Successfully Translated to Physical Address | Page Hit", va, pd_index, pt_index, offset);
        return vm->pd[pd_index].page_table[pt_index].page_frame[offset];
    } else {
        // Page fault handling
        vm->page_misses++;
        snprintf(vm->event_logs[vm->event_log_count++], 150, "LOAD at VA: 0x%X | PD Index: %d, PT Index: %d, Offset: %d | Cannot Translate - Page Miss", va, pd_index, pt_index, offset);
        handle_page_fault(vm, va);
        // After handling the page fault, it will be a hit
        snprintf(vm->event_logs[vm->event_log_count++], 150, "LOAD at VA: 0x%X resolved as Page Hit after page fault resolution", va);
        return load(vm, va);  // Retry after handling the page fault
    }
}

// Simulate storing data to a virtual address
void store(VirtualMemory *vm, uint32_t va, uint8_t value) {
    uint32_t pd_index = get_pd_index(va);
    uint32_t pt_index = get_pt_index(va);
    uint32_t offset = get_offset(va);

    // Check if PD and PT entries are valid
    if (vm->pd[pd_index].page_table != NULL &&
        vm->pd[pd_index].page_table[pt_index].page_frame != NULL) {
        vm->page_hits++;
        snprintf(vm->event_logs[vm->event_log_count++], 150, "STORE at VA: 0x%X | PD Index: %d, PT Index: %d, Offset: %d | Successfully Translated to Physical Address | Page Hit", va, pd_index, pt_index, offset);
        vm->pd[pd_index].page_table[pt_index].page_frame[offset] = value;
    } else {
        // Page fault handling
        vm->page_misses++;
        snprintf(vm->event_logs[vm->event_log_count++], 150, "STORE at VA: 0x%X | PD Index: %d, PT Index: %d, Offset: %d | Cannot Translate - Page Miss", va, pd_index, pt_index, offset);
        handle_page_fault(vm, va);
        // After handling the page fault, it will be a hit
        snprintf(vm->event_logs[vm->event_log_count++], 150, "STORE at VA: 0x%X resolved as Page Hit after page fault resolution", va);
        store(vm, va, value);  // Retry after handling the page fault
    }
}

// Free allocated memory
void cleanup_vm(VirtualMemory *vm) {
    for (int i = 0; i < PD_ENTRIES; i++) {
        if (vm->pd[i].page_table != NULL) {
            for (int j = 0; j < PT_ENTRIES; j++) {
                if (vm->pd[i].page_table[j].page_frame != NULL) {
                    free(vm->pd[i].page_table[j].page_frame);
                }
            }
            free(vm->pd[i].page_table);
        }
    }
    free(vm->memory);
}

// Print summary of the translation process
void print_summary(VirtualMemory *vm) {
    size_t page_directory_size = PD_ENTRIES * sizeof(PageDirectoryEntry);
    size_t page_table_size = PT_ENTRIES * sizeof(PageTableEntry);
    size_t page_table_entry_size = 4;  // Each page table entry is 4 bytes
    size_t page_table_total_size = page_table_entry_size * PT_ENTRIES;  // Total page table size is 4 KB

    printf("\nSummary:\n");
    printf("Total page hits: %d\n", vm->page_hits);
    printf("Total page misses: %d\n", vm->page_misses);
    printf("Total page faults: %d\n", vm->page_faults);  // Print the count of page faults
    printf("Hit/Miss ratio: %.2f\n", (float)vm->page_hits / (vm->page_hits + vm->page_misses));
    printf("Page Directory Size: %zu bytes\n", page_directory_size);
    printf("Page Table Size: %zu bytes per page table\n", page_table_total_size);  // Print size of page table as 4 KB

    printf("\nEvent Details (Chronological Order):\n");
    for (int i = 0; i < vm->event_log_count; i++) {
        printf("%s\n", vm->event_logs[i]);
    }
}


int main() {
    VirtualMemory vm;
    size_t memory_size = PAGE_SIZE * PT_ENTRIES * PD_ENTRIES;  // Simulate large memory
    init_vm(&vm, memory_size);

    // Test case: single virtual address in hexadecimal 0xAABB
    uint32_t test_va = 0xAABBCCDD;

    // Simulate loading data from the virtual address
    printf("Loading data from virtual address 0x%X...\n", test_va);
    uint8_t data = load(&vm, test_va);
    printf("Data loaded: 0x%X\n", data);

    // Simulate storing data to the virtual address
    printf("Storing data 0x12 at virtual address 0x%X...\n", test_va);
    store(&vm, test_va, 0x12);

    // Simulate loading data again from the virtual address
    printf("Loading data again from virtual address 0x%X...\n", test_va);
    data = load(&vm, test_va);
    printf("Data loaded: 0x%X\n", data);

    // Print the summary of translation and events
    print_summary(&vm);

    // Clean up allocated memory
    cleanup_vm(&vm);

    return 0;
}
