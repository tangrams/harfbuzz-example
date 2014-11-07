CC=clang++

FLAGS = \
	-g -O0 -fno-builtin -std=c++11 -O2 -Wall --pedantic \
	`freetype-config --cflags` \
	-I/usr/local/include/harfbuzz \
	-I/usr/local/include/ 
	
LIBS = \
	`freetype-config --libs` \
	-lglfw3 \
	-lglew \
	-framework OpenGL \
	-framework Cocoa

EXE = harfbuzz-ex.out

SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.h)
OBJECTS := $(SOURCES:.cpp=.o)

%.o : %.cpp
	@echo "$(CC) $@"
	@$(CC) -c $(FLAGS) $< -o $@

$(EXE): $(OBJECTS) $(HEADERS)
	@mkdir -p bin
	@echo "$(CC) $(OBJECTS) bin/$@"
	@$(CC) $(OBJECTS) $(LIBS) -o bin/$@ 

clean:
	@rm -rvf $(EXE) src/*.o bin