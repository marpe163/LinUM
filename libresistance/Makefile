
CC = gcc
CFLAGS = -Wall
TARGET = testprog
LIBS = libresistance.so


all: $(TARGET)

appl: test.o

lib: $(LIBS)

$(TARGET): lib appl
	$(CC) -o $@ test.o -L. -lresistance -Wl,-rpath,.

libresistance.so: resistance.o
	$(CC) -shared -fPIC -o libresistance.so $<

test.o: test.c
	$(CC) $(CFLAGS) -c $< -o test.o

resistance.o: resistance.c lib_resistance.h
	$(CC) $(CFLAGS) -c -fPIC $<

clean:
	-rm -f *.o $(TARGET)
	-rm -f *.so
