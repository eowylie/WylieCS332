# Makefile for Lab 5
# CS 332

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = lab5
SOURCE = lab5.c

# Default target
all: $(TARGET)

# Compile the program
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean up files
clean:
	rm -f $(TARGET) *.o

# Test the program with current directory
test: $(TARGET)
	./$(TARGET) .

# Test with a specific directory
test-home: $(TARGET)
	./$(TARGET) $(HOME)

# Install target
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall target
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
