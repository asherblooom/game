TARGET_EXEC := game
CXX:=g++
CC:=gcc
INC_DIR:=lib
CXXFLAGS:=-I$(INC_DIR) -Wall -Wextra -Wno-unused-parameter -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -std=c++20
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

$(TARGET_EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# find sources not in base directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/*/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -c
$(OBJ_DIR)/%.o: $(SRC_DIR)/*/*/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -c

# find sources in base directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -c

obj: 
	mkdir -p obj


EXAMPLES_DIR := ./examples
EXAMPLES_OBJ_DIR := $(EXAMPLES_DIR)/obj
_OBJS_EXAMPLES := $(patsubst %.cpp,%,$(notdir $(shell find $(EXAMPLES_DIR)/src -name '*.cpp')))
OBJS_EXAMPLES := $(_OBJS_EXAMPLES:%=$(EXAMPLES_DIR)/%)

_OBJS_EXAMPLES_LIBS := $(patsubst %.cpp,%.o,$(notdir $(shell find $(EXAMPLES_DIR)/lib -name '*.cpp')))
OBJS_EXAMPLES_LIBS := $(_OBJS_EXAMPLES_LIBS:%=$(EXAMPLES_OBJ_DIR)/%)

.PHONY: examples

examples: $(EXAMPLES_OBJ_DIR) $(OBJS_EXAMPLES)

$(EXAMPLES_DIR)/%: $(EXAMPLES_DIR)/src/%.cpp $(OBJS_EXAMPLES_LIBS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(EXAMPLES_OBJ_DIR)/%.o: $(EXAMPLES_DIR)/lib/%.cpp
	$(CXX) -o $@ $< -I$(INC_DIR) -c

$(EXAMPLES_OBJ_DIR):
	mkdir -p $(EXAMPLES_OBJ_DIR)


.PHONY: clean
# remove object dir and target exec and
# remove everything in EXAMPLES_DIR that isn't EXAMPLES_DIR/src or EXAMPLES_DIR/media or EXAMPLES_DIR/lib
clean:
	rm -rf $(OBJ_DIR) $(EXAMPLES_OBJ_DIR) $(TARGET_EXEC)
	rm -f $(filter-out $(EXAMPLES_DIR)/src $(EXAMPLES_DIR)/media $(EXAMPLES_DIR)/lib, $(wildcard $(EXAMPLES_DIR)/*))



