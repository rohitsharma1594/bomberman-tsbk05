# Compiler
CC = gcc

# Linker
LD = gcc

# Compiler flags go here.
CFLAGS = -I/usr/include/GL -I/usr/include/SDL

# Linker flags go here. Currently there aren't any, but if we'll switch to
# code optimization, we might add "-s" here to strip debug info and symbols.
LDFLAGS = -I/usr/include/GL -lglut `sdl-config --libs --cflags`

# Erase files.
RM = /bin/rm -f

# If there are files we dont want to compile, 
# then list the sources manually.
# ex. SOURCES=object.cpp model.cpp cube.cpp bomberman.cpp
SOURCES=$(wildcard *.cpp)

# Object directory
ODIR = obj

# List of generated object files.
_OBJS = $(SOURCES:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

# Program executable file name.
PROG = bomberman

# Top-level rule, to compile everything.
all: $(PROG)

# Rule to link the program
$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(PROG)

# Meta-rule for compiling any "cpp" source file.
$(ODIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule for cleaning re-compilable files.
clean:
	$(RM) $(PROG) $(OBJS)

# Rule for building dependency lists, and writing them to a file
# named ".depend".
depend:
	$(RM) .depend
	makedepend -f- -- $(CFLAGS) -- $(SRCS) > .depend

# Now add a line to include the dependency list.
include .depend

