# Compiler and Flags
CC = gcc
CFLAGS = -g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable -lpthread

# Source Files
SOURCES = buddhabrot.c read_ppm.c write_ppm.c
HEADERS = read_ppm.h write_ppm.h

# Target Executable
TARGET = buddhabrot

# Default Target
all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# Clean Up
clean:
	rm -f $(TARGET)
