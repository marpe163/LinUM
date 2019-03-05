CC = gcc
CFLAGS = -Wall -W -Wextra -pedantic
TARGET = electrotest
LIBS = libresistance.so libpower.so libcomponent.so
LIB_BUILDDIR = lib

DESTDIR ?= /usr/local
BIN_INSTDIR = $(DESTDIR)/bin
LIB_INSTDIR = $(DESTDIR)/lib

.PHONY: all local appl lib local_target lib_prepare clean uninstall install

all: $(TARGET)

local: local_target

appl: electrotest.o

lib: libs_prepare $(LIBS)

$(TARGET): lib appl
	$(CC) -o $@ electrotest.o -L./lib -lcomponent -lpower -lresistance -Wl,-rpath,$(LIB_INSTDIR)

local_target: lib appl
	$(CC) -o $(TARGET) electrotest.o -L./lib -lcomponent -lpower -lresistance -Wl,-rpath,./lib

libs_prepare:
	mkdir -p $(LIB_BUILDDIR)

libresistance.so: resistance.o
	$(CC) -shared -fPIC -o lib/libresistance.so $<

libpower.so: libpower.o
	$(CC) -shared -fPIC -o lib/libpower.so $<

libcomponent.so: libcomponent.o
	$(CC) -shared -fPIC -o lib/libcomponent.so $< -lm

electrotest.o: electrotest.c
	$(CC) $(CFLAGS) -D_GNU_SOURCE -c $< -o electrotest.o

resistance.o: libresistance/resistance.c libresistance/lib_resistance.h
	$(CC) $(CFLAGS) -c -fPIC $<

libpower.o: libpower/libpower.c libpower/libpower.h
	$(CC) $(CFLAGS) -c -fPIC $<

libcomponent.o: libcomponent/libcomponent.c libcomponent/libcomponent.h
	$(CC) $(CFLAGS) -DHAVE_MAIN -c -fPIC $<

clean:
	-rm -f *.o $(TARGET)
	-rm -f lib/*.so

uninstall: clean
	-rm -f $(BIN_INSTDIR)/$(TARGET)
	-rm -f $(LIB_INSTDIR)/libresistance.so
	-rm -f $(LIB_INSTDIR)/libpower.so
	-rm -f $(LIB_INSTDIR)/libcomponent.so

install: $(TARGET)
	-mkdir -p $(BIN_INSTDIR) $(LIB_INSTDIR)
	-install $(TARGET) $(BIN_INSTDIR)
	-install $(addprefix $(LIB_BUILDDIR)/,$(LIBS)) $(LIB_INSTDIR)
