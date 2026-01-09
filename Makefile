TARGET_EXEC := game
CXX:=g++
CC:=gcc
INC_DIR:=lib
MEMORY_DEBUG_FLAGS := -fsanitize=address -static-libasan
CXXFLAGS:=-I$(INC_DIR) $(shell pkg-config --cflags freetype2 openal) -march=native -Wall -Wextra -Wno-unused-parameter -std=c++20 
LINKERFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl $(shell pkg-config --libs freetype2 openal) -lvorbisfile -lvorbis -logg 
OBJ_DIR:=./obj
SRC_DIR:=./src

# Find all the C++ files we want to compile
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
# get a list of object files we want to compile by removing the 
# paths from the source files and then substituing .cpp for .o
_OBJS := $(patsubst %.cpp,%.o,$(notdir $(SRCS)))
# add the object directory to the front of the object files
OBJS := $(_OBJS:%=$(OBJ_DIR)/%)


.PHONY: main

main: obj $(TARGET_EXEC)

# link object files
$(TARGET_EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(LINKERFLAGS)

# find sources not in base directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/*/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -c
$(OBJ_DIR)/%.o: $(SRC_DIR)/*/*/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -c
$(OBJ_DIR)/%.o: $(SRC_DIR)/*/*/*/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -c

# find sources in base directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -c

obj: 
	mkdir -p obj


.PHONY: clean

# remove object dir and target exec
clean:
	rm -rf $(OBJ_DIR) $(TARGET_EXEC)



