CC=gcc
CFLAGS=-std=c11 -O2 -Wall -Wextra
LDLIBS=-lopenblas -llapacke -lm

TARGET=exercise1

all: $(TARGET)

$(TARGET): main.o matrix.o dense_solvers.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c matrix.h dense_solvers.h
	$(CC) $(CFLAGS) -c $<

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c $<

dense_solvers.o: dense_solvers.c dense_solvers.h
	$(CC) $(CFLAGS) -c $<

run: all
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)