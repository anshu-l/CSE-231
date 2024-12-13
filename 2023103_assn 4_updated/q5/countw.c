#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, char *argv[]) {

    int saare_words = 0 ;
    char abhi_wala_character;
    char ek_peeche_wala_character;

    if (argc == 2) {

        FILE *file = fopen(argv[1], "r");

        if (file == NULL) { printf ("galat file hai babu\n"); return 1;}


        while ((abhi_wala_character = fgetc(file)) != EOF) {
            
            if (isspace(abhi_wala_character)) {
                ek_peeche_wala_character = 0;
            }
            else if (!ek_peeche_wala_character) {
                ek_peeche_wala_character = 1;
                saare_words ++;
            }

        }

        printf("Aapki file mei : %d  words hai\n", saare_words);
        fclose(file);


    }

    else { printf("bhai mere dhang se input dede\n");}

    return 0;
    

}