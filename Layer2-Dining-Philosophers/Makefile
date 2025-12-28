CC = gcc
CFLAGS = -pthread -Wall

# Target: Build only the dining program
dining: philosophers.c
	$(CC) $(CFLAGS) dining_philosophers.c -o dining

# Clean up
clean:
	rm -f dining
