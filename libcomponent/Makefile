all: test
libcomponent.o: libcomponent.c libcomponent.h
	gcc -c -fPIC libcomponent.c
libcomponent.so: libcomponent.o
	gcc -shared libcomponent.o -o libcomponent.so
test: libcomponent.so libcomponenttest.c
	gcc libcomponenttest.c -L. -Wl,-rpath,. -lcomponent -lm -o test
clean:
	rm *.o
	rm *.so
install: all
	sudo mv libcomponent.so /usr/local/lib/
uninstall:
	sudo rm /usr/local/lib/libcomponent.so 
