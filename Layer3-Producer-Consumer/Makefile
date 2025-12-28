# Compiler and Flags
CC = gcc
CFLAGS = -pthread -Wall

# Target: Build the producer-consumer program
prodcon: producer_consumer.c
	$(CC) $(CFLAGS) producer_consumer.c -o prodcon

# Clean up (delete the executable)
clean:
	rm -f prodcon
