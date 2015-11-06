CC=g++
CFLAGS=-c -Wno-reorder -Wall -std=gnu++0x
LDFLAGS=
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	   $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	   $(CC) $(CFLAGS) $< -o $@
clean:
	rm -f *.o
	rm $(EXECUTABLE)
