# Makefile for Lab 7 
# CS 332
# Ethan Wylie - eowylie

CC = gcc
CFLAGS = -Wall -g
TARGET = forkexecvp_2
SOURCE = forkexecvp_2.c

# Default target
all: $(TARGET)

# Compile the main program
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean up compiled files
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
