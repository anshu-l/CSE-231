#include <stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

// Function to get the name of the month using switch statement
const char* month_name(int month) {
    switch(month) {
        case 1: return "January";
        case 2: return "February";
        case 3: return "March";
        case 4: return "April";
        case 5: return "May";
        case 6: return "June";
        case 7: return "July";
        case 8: return "August";
        case 9: return "September";
        case 10: return "October";
        case 11: return "November";
        case 12: return "December";
        default: return "Invalid Month";
    }
}

// Function to determine if a year is a leap year using if-else
int isLeapYear(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }

}


// Function to get the number of days in a month
int number_of_days_in_month(int month, int year) {
    static const int daysInMonth[] =  {31,
                                       28,
                                       31, 
                                       30, 
                                       31, 
                                       30, 
                                       31, 
                                       31, 
                                       30, 
                                       31, 
                                       30, 
                                       31};

    if (month == 2 && isLeapYear(year))
        return 29;
    return daysInMonth[month - 1];
}

// Function to calculate the day of the week using Zeller's congruence
int calculatingdayofaweek(int month, int year) {
    if (month == 1 || month == 2) {
        month += 12;
        year--;
    }
    
    int k = year % 100;
    int j = year / 100;
    int dayOfWeek = (1 + 13*(month+1)/5 + k + k/4 + j/4 + 5*j) % 7;
    
    return (dayOfWeek + 6) % 7; // Adjust to make Sunday 0, Monday 1, ..., Saturday 6
}
// Function to print the calendar
void printCalendar(int month, int year) {
    printf("    %s %d\n", month_name(month), year);
    printf("Su Mo Tu We Th Fr Sa \n");

    int dayOfWeek = calculatingdayofaweek(month, year);
    int daysInMonth = number_of_days_in_month(month, year);
    for (int i = 0; i < dayOfWeek; i++) {
        printf("   ");
    }
    for (int day = 1; day < (daysInMonth+1); day++) {
        printf("%2d ", day);
        if (++dayOfWeek > 6) {
            dayOfWeek = 0;

            printf("\n");
        }
    }

    if (dayOfWeek != 0) {
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <month> <year>\n", argv[0]);
        return 1;
    }

    int month = atoi(argv[1]);
    int year = atoi(argv[2]);

    if (month < 1 || month > 12 || year < 1) {
        fprintf(stderr, "Invalid month or year\n");
        return 1;
    }

    printCalendar(month, year);
    return 0;
}
