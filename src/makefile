CC = gcc
FLAGS = -Wall -Wextra -O3
MODE = -g 
TARGET = cifras

$(TARGET): cifras.o functionsf1.o
	$(CC) $(FLAGS) $(MODE) -o $(TARGET) cifras.o functionsf1.o

cifras.o: cifras.c cifras.h
	$(CC) $(FLAGS) $(MODE) -c cifras.c

functionsf1.o: functionsf1.c cifras.h
	$(CC) $(FLAGS) $(MODE) -c functionsf1.c

clean:
	rm *.o $(TARGET)