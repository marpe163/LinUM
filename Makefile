CC = gcc
CFLAGS = -Wall -W -Wextra -pedantic
TARGET = electrotest
LIBS = libresistance.so libpower.so libcomponent.so
BIN_INSTDIR = /usr/local/bin
LIB_INSTDIR = /usr/local/lib
LIB_BUILDDIR = lib

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
	$(CC) -shared -fPIC -lm -o lib/libpower.so $<

libcomponent.so: libcomponent.o
	$(CC) -shared -fPIC -o lib/libcomponent.so $<

electrotest.o: electrotest.c
	$(CC) $(CFLAGS) -D_GNU_SOURCE -c $< -o electrotest.o

resistance.o: libresistance/resistance.c libresistance/lib_resistance.h
	$(CC) $(CFLAGS) -c -fPIC $<

libpower.o: libpower/libpower.c libpower/libpower.h
	$(CC) $(CFLAGS) -c -fPIC $<

libcomponent.o: scr/libcomponent/libcomponent.c
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
	@if [ -d $(BIN_INSTDIR) ]; \
		then \
		cp $(TARGET) $(BIN_INSTDIR) &&\
		chmod a+x $(BIN_INSTDIR)/$(TARGET) &&\
		chmod og-w $(BIN_INSTDIR)/$(TARGET) &&\
		echo "Installed $(TARGET) in $(BIN_INSTDIR)" ;\
	else \
		echo "Directory $(BIN_INSTDIR) does not exist" ; false ;\
	fi
	@if [ -d $(LIB_INSTDIR) ]; \
		then \
		# cp lib/$(LIBS) $(LIB_INSTDIR) &&\
		cp lib/libpower.so $(LIB_INSTDIR) &&\
		cp lib/libresistance.so $(LIB_INSTDIR) &&\
		cp lib/libcomponent.so $(LIB_INSTDIR) &&\
		chmod a+x $(LIB_INSTDIR)/libpower.so &&\
		chmod a+x $(LIB_INSTDIR)/libresistance.so &&\
		chmod a+x $(LIB_INSTDIR)/libcomponent.so &&\
		chmod og-w $(LIB_INSTDIR)/libpower.so &&\
		chmod og-w $(LIB_INSTDIR)/libresistance.so &&\
		chmod og-w $(LIB_INSTDIR)/libcomponent.so &&\
		echo "Installed $(LIBS) in $(LIB_INSTDIR)" ;\
	else \
		echo "Directory $(LIB_INSTDIR) does not exist" ; false ;\
	fi
