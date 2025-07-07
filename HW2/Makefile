# Makefile for search program
# CS 332/532 Systems Programming - HW 2

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = search
SOURCES = search.c

# Default target
all: $(TARGET)

# Build the search program
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Install (copy to /usr/local/bin - optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Test the program with sample directory
test: $(TARGET)
	@echo "Testing search program..."
	@echo "1. Basic directory listing:"
	./$(TARGET) projects
	@echo "\n2. With -S option:"
	./$(TARGET) -S projects
	@echo "\n3. With size filter:"
	./$(TARGET) -s 1024 projects
	@echo "\n4. With pattern and depth filter:"
	./$(TARGET) -f docx 2 projects

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the search program (default)"
	@echo "  clean   - Remove built files"
	@echo "  test    - Run basic tests"
	@echo "  install - Install to /usr/local/bin"
	@echo "  help    - Show this help message"

.PHONY: all clean test install help
