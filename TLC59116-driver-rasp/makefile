# ---------------------------------------------------------------------------
# This software is in the public domain, furnished "as is", without technical
# support, and with no warranty, express or implied, as to its usefulness for
# any purpose.
#
# Author: Matthieu Holtz
# Year: 2015
# ---------------------------------------------------------------------------

CC=g++

CPPFLAGS= -Wall -std=c++0x -std=gnu++0x -pedantic -O2

all: tlc

tlc: main.o I2C.o TLC59116.o
	$(CC) $(CPPFLAGS) main.o I2C.o TLC59116.o -o tlc

main.o: main.cpp
	$(CC) $(CPPFLAGS) -c main.cpp

I2C.o: I2C.cpp
	$(CC) $(CPPFLAGS) -c I2C.cpp

TLC59116.o: TLC59116.cpp
	$(CC) $(CPPFLAGS) -c TLC59116.cpp

clean:
	rm *o tlc