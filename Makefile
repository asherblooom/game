TARGET_EXEC := game
CXX:=g++
CC:=gcc
INC_DIR:=include/
CXXFLAGS:=-I$(INC_DIR) -Wall -Wextra -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OBJ_DIR:=./obj
SRC_DIR:=./src

GLAD_OBJ := $(OBJ_DIR)/glad.o
GLAD_SRC := $(SRC_DIR)/glad.c

DDS_OBJ := $(OBJ_DIR)/dds.o
DDS_SRC := $(SRC_DIR)/dds.c

# Find all the C++ files we want to compile
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
# get a list of object files we want to compile by removing the 
# paths from the source files and then substituing .cpp for .o
_OBJS := $(patsubst %.cpp,%.o,$(notdir $(SRCS)))
# add the object directory to the front of the object files
OBJS := $(_OBJS:%=$(OBJ_DIR)/%)

main: obj $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS) $(GLAD_OBJ) $(DDS_OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) -c

obj: 
	mkdir -p obj

EXAMPLES_DIR := ./examples
_OBJS_EXAMPLES := $(patsubst %.cpp,%,$(notdir $(shell find ./examples/src -name '*.cpp')))
OBJS_EXAMPLES := $(_OBJS_EXAMPLES:%=$(EXAMPLES_DIR)/%)


.PHONY: examples

examples: obj $(OBJS_EXAMPLES)

$(EXAMPLES_DIR)/%: $(EXAMPLES_DIR)/src/%.cpp $(GLAD_OBJ) $(DDS_OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# compiles the glad library
$(GLAD_OBJ): $(GLAD_SRC)
	$(CC) -I$(INC_DIR) $< -o $@ -c

# compiles the dds loader
$(DDS_OBJ): $(DDS_SRC)
	$(CC) -I$(INC_DIR) $< -o $@ -c


.PHONY: clean

clean:
	rm -rf $(OBJ_DIR) game
	# remove everything in EXAMPLES_DIR that isn't EXAMPLES_DIR/src or EXAMPLES_DIR/media
	rm -f $(filter-out $(EXAMPLES_DIR)/src $(EXAMPLES_DIR)/media, $(wildcard $(EXAMPLES_DIR)/*))



