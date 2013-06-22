CC=g++
CPPFLAGS=-m64 -std=c++11 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wno-switch -O3 -g -pg
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp,obj/%.o,$(wildcard *.cpp))

all: $(OBJ)
	mkdir -p bin/
	$(CC) $(LDFLAGS) $(OBJ) -o bin/sfml-test.bin

obj/%.o: %.cpp
	mkdir -p obj/
	$(CC) $(CPPFLAGS) -c $< -o $@

test:
	echo -e "\nCC=$(CC)\nCPPFLAGS=$(CPPFLAGS)\nLDFLAGS=$(LDFLAGS)\nSRC=$(SRC)\nOBJ=$(OBJ)\n"

clean:
	rm -rfv bin/ obj/
