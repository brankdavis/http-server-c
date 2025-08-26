# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
SRC = tests.c ../src/utils/utils.c ../src/server/helpers/helpers.c

# Output binary name
TARGET = test

# Default target
all: $(TARGET)

# Build the test executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run the tests
test: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)
