CPP_COMPILER ?= clang++

CPP_COMPILATION_ARGS += -Wall -Werror -std=c++17
CPP_COMPILATION_ARGS += $(shell pkg-config --cflags gtk4)
CPP_COMPILATION_ARGS += -I./inc

CPP_LINKER_ARGS += -Wall -Werror -std=c++17
CPP_LINKER_ARGS += $(shell pkg-config --libs gtk4)

CPP_SOURCES += $(shell find ./src -name "*.cpp")

OBJECTS += $(CPP_SOURCES:.cpp=.o)

%.o: %.cpp
	$(CPP_COMPILER) $(CPP_COMPILATION_ARGS) -c $< -o $@

all: $(OBJECTS)
	$(CPP_COMPILER) $(OBJECTS) $(CPP_LINKER_ARGS) -o main.o
clean:
	rm -rf main.o $(OBJECTS)
