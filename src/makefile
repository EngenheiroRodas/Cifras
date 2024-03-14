CC = gcc
CFLAGS = -Wall -Wextra 

# Source files
SRCS = cifras.c functions.c
OBJS = $(SRCS:.c=.o)

# Header files
HDRS = functions.h

# Executable name
EXEC = cifras

# Default target
all: $(EXEC)

# Linking step
$(EXEC): $(OBJS)
    $(CC) $(CFLAGS) $^ -o $@

# Compilation step for source files
%.o: %.c $(HDRS)
    $(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
    rm -f $(EXEC) $(OBJS)