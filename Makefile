#
# Makefile
#

all: dbus-test 

%.o: %.cpp
	gcc -Wall -c $< `pkg-config --cflags dbus-1`

dbus-test: DBus_Base.o main.o
	g++ DBus_Base.o main.o -o dbus-test `pkg-config --libs dbus-1` -lpthread

.PHONY: clean $(all)
clean:
	rm *.o dbus-test 
