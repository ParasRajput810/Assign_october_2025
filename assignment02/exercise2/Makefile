# Makefile for assignment2/exercise2

CC      = gcc
CFLAGS  = -O3 -Wall -Wextra -std=c11
LDLIBS  = -lm

TARGET  = numerical_diff
OBJS    = numerical_diff.o

.PHONY: all clean docs

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

numerical_diff.o: numerical_diff.c

docs:
	doxygen Doxyfile

clean:
	rm -f $(TARGET) $(OBJS) numerical_diff.txt
