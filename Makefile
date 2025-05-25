TARGET_EXEC := game
CXX:=g++
INC_DIR:=include/
CXXFLAGS:=-I$(INC_DIR) -Wall -Wextra -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OBJ_DIR:=./obj
SRC_DIR:=./src

GLAD_OBJ := $(OBJ_DIR)/glad.o
GLAD_SRC := $(SRC_DIR)/glad.c

# Find all the C++ files we want to compile
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
# get a list of object files we want to compile by removing the 
# paths from the source files and then substituing .cpp for .o
_OBJS := $(patsubst %.cpp,%.o,$(notdir $(SRCS)))
# add the object directory to the front of the object files
OBJS := $(_OBJS:%=$(OBJ_DIR)/%)


game: $(OBJS) $(GLAD_OBJ)
	$(CXX) -o $(TARGET_EXEC) $^ $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp obj
	$(CXX) -o $@ $< $(CXXFLAGS) -c

obj: 
	mkdir -p obj

EXAMPLES_DIR := ./examples
_OBJS_EXAMPLES := $(patsubst %.cpp,%.o,$(notdir $(shell find ./examples/src -name '*.cpp')))
OBJS_EXAMPLES := $(_OBJS_EXAMPLES:%=$(EXAMPLES_DIR)/%)

examples : $(OBJS_EXAMPLES)

$(EXAMPLES_DIR)/%.o : $(EXAMPLES_DIR)/src/%.cpp $(GLAD_OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# compiles the glad library
$(GLAD_OBJ): $(GLAD_SRC) 
	g++ -I$(INC_DIR) $(GLAD_SRC) -o $(GLAD_OBJ) -c
