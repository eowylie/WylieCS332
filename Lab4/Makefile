CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = lab4
SOURCE = lab4.c

# Default target
all: $(TARGET)

# Build the lab4 program
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Test the program with sample files
test: $(TARGET)
	@echo "Creating test files..."
	@echo "Hello World! This is file one!" > file1
	@echo "Hello World! This is file two!" > file2
	@echo "Contents of file1 before concatenation:"
	@cat file1
	@echo "Contents of file2:"
	@cat file2
	@echo "Running concatenation..."
	./$(TARGET) file1 file2
	@echo "Contents of file1 after concatenation:"
	@cat file1
	@echo "Contents of file2 (should be unchanged):"
	@cat file2

# Test error conditions
test-errors: $(TARGET)
	@echo "Testing error conditions..."
	@echo "1. Testing same filename error:"
	-./$(TARGET) file1 file1
	@echo "2. Testing missing arguments:"
	-./$(TARGET) file1
	@echo "3. Testing non-existent file:"
	-./$(TARGET) file1 nonexistent.txt

# Clean test files
clean-test:
	rm -f file1 file2 test_large.txt

# Create large test file for performance testing
test-large: $(TARGET)
	@echo "Creating large test file (1MB)..."
	dd if=/dev/zero of=test_large.txt bs=1024 count=1024 2>/dev/null
	@echo "Original file1 size:"
	ls -lh file1
	./$(TARGET) file1 test_large.txt
	@echo "New file1 size after appending 1MB file:"
	ls -lh file1

# Help target
help:
	@echo "Available targets:"
	@echo "  all         - Build the lab4 program (default)"
	@echo "  clean       - Remove built files"
	@echo "  test        - Run basic functionality tests"
	@echo "  test-errors - Test error handling"
	@echo "  test-large  - Test with large file"
	@echo "  clean-test  - Remove test files"
	@echo "  help        - Show this help message"

.PHONY: all clean test test-errors clean-test test-large help
