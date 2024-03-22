CC = gcc
FLAGS = -Wall -Wextra -O3
MODE = -g 
TARGET = cifras
SRCS = cifras.c cryptor.c iofunctions.c statistics.c
OBJS = $(SRCS:.c=.o)

all-: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(MODE) -o $@ $^

%.o: %.c cifras.h
	$(CC) $(FLAGS) $(MODE) -c $< -o $@

clean:
	rm -f $(TARGET) *.o