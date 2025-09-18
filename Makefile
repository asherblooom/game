# Cross-platform Makefile for OpenGL + GLFW + GLAD
# =======
# TARGET_EXEC := game
# CXX:=g++
# CC:=gcc
# INC_DIR:=lib
# CXXFLAGS:=-I$(INC_DIR) -march=native -Wall -Wextra -Wno-unused-parameter -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl $(shell pkg-config --cflags --libs freetype2) -std=c++20
# OBJ_DIR:=./obj
# SRC_DIR:=./src
# >>>>>>> dev

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
FREETYPE_DIR := lib/freetype

# OS-specific setup for executable extension
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
endif

# Include directories
INC_DIRS := -Ilib/include -Ilib/include/freetype2

# Libraries
LIBS :=
ifeq ($(OS),Windows_NT)
    LIBS += -L$(GLFW_DIR)/lib -L$(FREETYPE_DIR)/lib -lglfw3 -lfreetype -lopengl32 -lgdi32
else
    LIBS += -lglfw -lGL -lX11 -lpthread -ldl -lXrandr -lXi $(shell pkg-config --cflags --libs freetype2)
endif

# Compiler flags
# CXXFLAGS for C++ files, CFLAGS for C files, JOINTFLAGS for both
JOINTFLAGS := -Wall -Wextra -Wno-unused-parameter $(INC_DIRS)
CXXFLAGS := $(JOINTFLAGS) -std=c++20
CFLAGS := $(JOINTFLAGS)

# Find all C++ files we want to compile (and all GLAD c files)
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)) $(wildcard $1$2)
CPP_SRCS := $(call rwildcard,$(SRC_DIR)/,*.cpp)
C_SRCS := $(call rwildcard,$(GLAD_DIR)/src/,*.c)

# Substitue .cpp for .o to generate object file names
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
