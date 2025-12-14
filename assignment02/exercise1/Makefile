# Makefile for assignment2/exercise1

CC      = gcc
CFLAGS  = -O3 -Wall -Wextra -std=c11

TARGET  = axpy_test
OBJS    = main.o axpy.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c axpy.h
axpy.o: axpy.c axpy.h

clean:
	rm -f $(OBJS) $(TARGET)
