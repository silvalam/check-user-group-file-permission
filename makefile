# makefile for access

all: access.cpp
	g++ -o access access.cpp

clean:
	$(RM) access
