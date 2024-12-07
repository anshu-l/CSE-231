#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int vpn;
    int pfn;
    bool protect_bits;
    bool valid;
} page_table;

int main() {
    page_table* all_pages = (page_table*)malloc(16 * sizeof(page_table));
    page_table* tlb = (page_table*)malloc(4 * sizeof(page_table));
    char str[100];
    int pointer = 0;

    for (int i = 0; i < 16; i++) {
        all_pages[i].vpn = i;
        all_pages[i].pfn = i + 1;
        all_pages[i].protect_bits = true;
        all_pages[i].valid = true;
    }

    for (int i = 0; i < 4; i++) {
        tlb[i].vpn = -1;
        tlb[i].pfn = -1;
        tlb[i].protect_bits = false;
        tlb[i].valid = false;
    }

    while (1) {
        printf("Enter the logical address (in hex): (enter -1 to stop)\n");
        fgets(str, sizeof(str), stdin);

        if (strncmp(str, "-1", 2) == 0) {
            break;
        }

        int logical_address = (int)strtol(str, NULL, 16);
        int vpn = (logical_address & 0xF000) >> 12;
        int offset = logical_address & 0x0FFF;
        bool success = false;

        for (int i = 0; i < 4; i++) {
            if (tlb[i].vpn == vpn && tlb[i].valid) {
                success = true;
                int pfn = tlb[i].pfn;
                if (tlb[i].protect_bits) {
                    int physical_address = (pfn << 12) | offset;
                    printf("TLB HIT!!\n");
                    printf("\tLogical Address: 0x%x\n\tPhysical Address: 0x%x\n", logical_address, physical_address);
                } else {
                    printf("PROTECTION FAULT\n");
                }
                break;
            }
        }

        if (!success) {
            printf("TLB MISS\n");
            int PTBR = 0x1000;  
            int PTEAddr = PTBR + (vpn * sizeof(page_table));  
            page_table PTE = all_pages[vpn];  

            if (!PTE.valid) {
                printf("SEGMENTATION FAULT\n");
            } else if (!PTE.protect_bits) {
                printf("PROTECTION FAULT\n");
            } else {
                tlb[pointer].vpn = vpn;
                tlb[pointer].pfn = PTE.pfn;
                tlb[pointer].protect_bits = PTE.protect_bits;
                tlb[pointer].valid = true;
                pointer = (pointer + 1) % 4;
                printf("Updated TLB: Logical Address: %d -> Physical Frame Number: %d\n", vpn, PTE.pfn);
            }
        }
        printf("---------------------------\n\n");
    }

    free(all_pages);
    free(tlb);
    return 0;
}
