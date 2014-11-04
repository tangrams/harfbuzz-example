CC=clang++
FLAGS = -g -O0 -fno-builtin -std=c++11 -O2 -Wall --pedantic `freetype-config --cflags` -I/usr/local/include/harfbuzz
LIBS =	-lharfbuzz \
		`freetype-config --libs` \
		-framework GLUT \
		-framework OpenGL \
		-framework Cocoa

EXE = harfbuzz-ex.out

SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJECTS := $(SOURCES:.cpp=.o)

%.o : %.cpp
	@echo "$(CC) $@"
	@$(CC) -c $(FLAGS) $< -o $@

$(EXE): $(OBJECTS) $(HEADERS)
	@echo "$(CC) $@"
	@$(CC) $(OBJECTS) $(LIBS) -o $@ 

clean:
	@rm -vf $(EXE) *.o