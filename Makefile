# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic

# Target executable
TARGET = chain

# Source files
SRCS = src/main.cpp src/MarkovChain.cpp

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
