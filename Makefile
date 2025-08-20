# Cross-platform Makefile for OpenGL + GLFW + GLAD

# Target executable
TARGET := game

# Compiler
CXX := g++
CC := gcc

# Directories
SRC_DIR := src
OBJ_DIR := obj
GLAD_DIR := lib/glad
GLFW_DIR := lib/glfw

# OS-specific setup for executable extension
ifeq ($(OS),Windows_NT)
    TARGET_EXT := .exe
else
    TARGET_EXT :=
endif
TARGET := $(TARGET)$(TARGET_EXT)

# Include directories
INC_DIRS := -I$(GLAD_DIR)/include -I$(GLFW_DIR)/include -I$(SRC_DIR) -Ilib

# Libraries
LIBS := -L$(GLFW_DIR)/lib
ifeq ($(OS),Windows_NT)
    LIBS += -lglfw3 -lopengl32 -lgdi32
else
    LIBS += -lglfw -lGL -lX11 -lpthread -ldl -lXrandr -lXcursor -lXinerama -lXxf86vm
endif

# Compiler flags
# CXXFLAGS for C++ files, CFLAGS for C files
CXXFLAGS := -Wall -Wextra -std=c++20 $(INC_DIRS)
CFLAGS   := -Wall -Wextra $(INC_DIRS)

# --- Robust File Finding ---
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)
CPP_SRCS := $(call rwildcard,$(SRC_DIR)/,*.cpp)
C_SRCS := $(call rwildcard,$(GLAD_DIR)/src/,*.c)

# --- Object File Generation ---
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRCS))
OBJS += $(patsubst $(GLAD_DIR)/src/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	@echo Linking $@...
	$(CXX) -o $@ $^ $(LIBS)

# --- Compilation Rules ---

# Rule for C++ files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p "$(dir $@)"
	@echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for C files (glad)
$(OBJ_DIR)/%.o: $(GLAD_DIR)/src/%.c
	@mkdir -p "$(dir $@)"
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	@echo Cleaning project...
	-rm -f $(TARGET)
	-rm -rf $(OBJ_DIR)

# Phony targets
.PHONY: all clean
