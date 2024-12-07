#include <stdio.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    struct sysinfo system_information;

    // Retrieve system information
    if (sysinfo(&system_information) != 0) 
    
    {
        // Error handling
        printf("Error : Failed to retrieve system uptime: %s\n", strerror(errno));
        return 1;
    }

    // Check for command-line arguments
    if (argc == 1) 
    
    {
        // Calculate uptime in hours, minutes, and seconds
        long uptime = system_information.uptime;
        int hours = uptime / 3600;
        int minutes = (uptime % 3600) / 60;
        int seconds = uptime % 60;

        // Print the uptime in HH:MM:SS format
        printf("%d:%d:%d\n", hours, minutes, seconds);
    } 
    else 
    
    {

        // Print usage instructions if arguments are provided
        printf("Usage : %s\n", argv[0]);
        return 1;
    }

    return 0;
}

