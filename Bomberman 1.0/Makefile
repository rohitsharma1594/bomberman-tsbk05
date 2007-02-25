# Define the symbols:
CC=g++
CFLAGS=-I/usr/include/GL -lglut `sdl-config --libs --cflags`
LDFLAGS=-I/usr/include/GL -lglut `sdl-config --libs --cflags`

#If there are files we dont want to compile. Then list them manually.
#SOURCES=object.cpp model.cpp cube.cpp bomberman.cpp

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=bomberman

all: $(EXECUTABLE)

$(EXECUTABLE): 
	$(CC) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

.PHONY : clean
clean :
	rm bomberman $(OBJECTS)

