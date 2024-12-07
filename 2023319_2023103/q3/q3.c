#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct Student {
    int id;           
    char name[20];    
};

int main() {
    int student_id_ki_talash;
    int file_descriptor;
    struct Student student_ka_record;
    off_t seek_position;

    printf("Student ka ID dalo (1 se 5 ke beech): ");
    scanf("%d", &student_id_ki_talash);

    if (student_id_ki_talash < 1 || student_id_ki_talash > 5) {
        printf("Galat ID! Pajama insaan ho kya ekdum.\n");
        return 1;
    }

    file_descriptor = open("students.rec", O_RDONLY);
    if (file_descriptor == -1) {
        printf("File kholne mein isskill issue aaya.\n");
        return 1;
    }

    
    seek_position = (student_id_ki_talash - 1) * sizeof(struct Student);

    if (lseek(file_descriptor, seek_position, SEEK_SET) == (off_t) -1) {
        printf("lseek mein skill issue aagya.\n");
        close(file_descriptor);
        return 1;
    }

    if (read(file_descriptor, &student_ka_record, sizeof(struct Student)) != sizeof(struct Student)) {
        printf("Record padhne mein skill issue aagayaaa.\n");
        close(file_descriptor);
        return 1;
    }

    printf("Student ka naam: %s\n", student_ka_record.name);

    close(file_descriptor);
    return 0;
}
