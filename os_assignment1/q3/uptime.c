#include <stdio.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    struct sysinfo system_information;

 
    if (sysinfo(&system_information) != 0) {
       
        printf("Error: Failed to retrieve system uptime: %s\n", strerror(errno));
        return 1;
    }

    if (argc == 1) {
        
        long uptime = system_information.uptime;
        int hours = uptime / 3600;
        int minutes = (uptime % 3600) / 60;
        int seconds = uptime % 60;

        // Print the uptime in HH:MM:SS farmat
        printf("%d:%d:%d\n", hours, minutes, seconds);
    } else {
        // Print usage instruction if arguments are provided
        printf("Usage: %s\n", argv[0]);
        return 1;
    }

    return 0;
}

