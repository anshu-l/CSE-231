#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


struct page {
    uint8_t page_number;
    bool reference_bit;
    bool dirty_bit;
};



int main() {
    int num_of_frames;
    char str[100];
    struct page current_req;
    char* token;
    int page_faults = 0;



    printf("Enter the page requests: ");
    fgets(str, sizeof(str), stdin);

    printf("Enter the number of frames: ");
    scanf("%d", &num_of_frames);



    struct page frames[num_of_frames];


    str[strcspn(str, "\n")] = 0;


    char temp_str[100];
    strcpy(temp_str, str); 
    
    int num_of_page_req = 0;
    token = strtok(temp_str, ",");
    while (token != NULL) {
        num_of_page_req++;
        token = strtok(NULL, ",");
    }
    struct page *midam = (struct page *)malloc(num_of_page_req*sizeof(struct page));



    int i = 0;
    strcpy(temp_str, str);
    token = strtok(temp_str, ",");

    for (int i = 0; i< num_of_page_req; i++) {

        midam[i].page_number = atoi(token);  
        midam[i].reference_bit = 0;          
        midam[i].dirty_bit = 0;              
        token = strtok(NULL, ",");         

    }

    strcpy(temp_str, str);
    token = strtok(temp_str, ",");

    for (int i = 0; i<num_of_frames; i++) {

        if (token != NULL) {

            frames[i] = midam[i];
            token = strtok(NULL, ",");
            page_faults ++;

        }

    }
// //////////////////////


    int count = 0 ;
    int clock_pointer = 0;
    bool reset = 0;

    bool found = 0;
    while(token != NULL ) {

        current_req = midam[num_of_frames + count];
        found = 0;
        reset = 0;

        for (int i = 0 ; i < num_of_frames; i ++ ) {

            if (frames[i].page_number == current_req.page_number) {

                found = 1;
                frames[i].reference_bit = 1;
                count ++;
                reset = 1;
            }

        }

        if (!found) {
            page_faults++;

            for (int i = clock_pointer; i < num_of_frames; i ++ ) {
                
                
                if (frames[i].reference_bit) {
                
                    frames[i].reference_bit = 0;
                
                }
                
                else {

                    frames[i] = current_req;
                    frames[i].reference_bit = 1;
                    reset = 1;
                    clock_pointer = i+1;
                    count++;
                    break;

                }

            }

            if (!reset) {

                clock_pointer = 0;

                for (int i = clock_pointer ; i < num_of_frames ; i ++ ) {
                    
                    if (!frames[i].reference_bit) {
                        
                        frames[i] = current_req;
                        frames[i].reference_bit = 1;
                        reset = 1;
                        clock_pointer = i+1;
                        count ++;
                        break;

                    }

                }
            }

        }

        token = strtok(NULL, ",");

    }

    printf("Page Faults: %d\n", page_faults);
    printf("Page hits: %d\n", num_of_page_req-page_faults);
    printf("Hit Rate: %.2f\n", (double)(num_of_page_req-page_faults)/num_of_page_req);

    return 0;


}


