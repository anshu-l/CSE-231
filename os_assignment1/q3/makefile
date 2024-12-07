# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall  # Removed -Werror

# Targets and their dependencies
.PHONY: all clean

all: date cal uptime main

date: date.c
	$(CC) $(CFLAGS) -o date date.c

cal: cal.c
	$(CC) $(CFLAGS) -o cal cal.c

uptime: uptime.c
	$(CC) $(CFLAGS) -o uptime uptime.c

main: main.c date cal uptime
	$(CC) $(CFLAGS) -o main main.c

# Clean up build files
clean:
	rm -f date cal uptime main


