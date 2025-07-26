# Makefile for HW3
# CS 332
# Ethan Wylie - eowylie
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
TARGET = hw3
SOURCE = hw3.c

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean target to remove compiled files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
