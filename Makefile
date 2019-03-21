CC = gcc
CFLAGS = -Wall -W -Wextra -pedantic
TARGET = electrotest
LIBS = libresistance.so libpower.so libcomponent.so
LIB_BUILDDIR = lib
BIN_BUILDDIR = bin

DESTDIR ?= /usr/local
BIN_INSTDIR = $(DESTDIR)/bin
LIB_INSTDIR = $(DESTDIR)/lib

.PHONY: all appl lib lib_prepare clean uninstall install

all: $(TARGET)

appl: electrotest.o

lib: libs_prepare $(LIBS)

$(TARGET): lib appl
	$(CC) -o $(BIN_BUILDDIR)/$(TARGET) electrotest.o -L./lib \
		-lcomponent -lpower -lresistance -Wl,-rpath,\$$ORIGIN/../lib

libs_prepare:
	mkdir -p $(LIB_BUILDDIR)
	mkdir -p $(BIN_BUILDDIR)

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
	-rm -f *.o $(BIN_BUILDDIR)/$(TARGET)
	-rm -f $(LIB_BUILDDIR)/*.so

uninstall: clean
	-rm -f $(BIN_INSTDIR)/$(TARGET)
	-rm -f $(LIB_INSTDIR)/libresistance.so
	-rm -f $(LIB_INSTDIR)/libpower.so
	-rm -f $(LIB_INSTDIR)/libcomponent.so

install: $(TARGET)
	-mkdir -p $(BIN_INSTDIR) $(LIB_INSTDIR)
	-install $(BIN_BUILDDIR)/$(TARGET) $(BIN_INSTDIR)
	-install $(addprefix $(LIB_BUILDDIR)/,$(LIBS)) $(LIB_INSTDIR)
