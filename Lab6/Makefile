# Makefile for Lab 6 CS 332

CC = gcc
CFLAGS = -Wall -O
TARGET = lab6
SOURCE = lab6.c

# Default target
all: $(TARGET)

# Compile the main program
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean up compiled files
clean:
	rm -f $(TARGET) output.log

# Test with sample input
test: $(TARGET)
	@echo "Creating sample input file..."
	@echo "uname -a" > sample_input.txt
	@echo "date" >> sample_input.txt
	@echo "whoami" >> sample_input.txt
	@echo "ls -la" >> sample_input.txt
	@echo "Running lab6 with sample input..."
	./$(TARGET) sample_input.txt
	@echo "Check output.log for results"
