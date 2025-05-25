TARGET_EXEC := game
CXX:=g++
INC_DIR:=include/
CXXFLAGS:=-I$(INC_DIR) -Wall -Wextra -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
OBJ_DIR:=./obj/
SRC_DIR:=./src/
GLAD_OBJ := ./obj/glad.o

# Find all the C++ files we want to compile
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

OBJS := $(patsubst %.cpp,%.o,$(notdir $(SRCS)))


linux: $(OBJS) $(GLAD_OBJ)
	$(CXX) -o $(TARGET_EXEC) $(OBJ_DIR)$< $(GLAD_OBJ) $(CXXFLAGS)

%.o: ./src/%.cpp mkobjdir
	$(CXX) -o $(OBJ_DIR)$@ $< $(CXXFLAGS) -c

mkobjdir: 
	mkdir -p obj

OBJS_EXAMPLES := $(patsubst %.cpp,%.o,$(notdir $(shell find ./examples/src -name '*.cpp')))
examples : $(OBJS_EXAMPLES)

%.o : ./examples/src/%.cpp $(GLAD_OBJ)
	$(CXX) -o ./examples/$@ $^ $(CXXFLAGS)

# compiles the glad library
$(GLAD_OBJ): src/glad.c 
	g++ -Iinclude/ src/glad.c -o $(GLAD_OBJ) -c
