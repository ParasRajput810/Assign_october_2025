CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra
LDLIBS = -lm

TARGET = exercise2

all: $(TARGET)

$(TARGET): main.o coo.o solvers.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c coo.h solvers.h
	$(CC) $(CFLAGS) -c $<

coo.o: coo.c coo.h
	$(CC) $(CFLAGS) -c $<

solvers.o: solvers.c solvers.h coo.h
	$(CC) $(CFLAGS) -c $<

run: all
	./$(TARGET) test_mat.txt

clean:
	rm -f *.o $(TARGET)