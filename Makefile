######################################################################
# This makefile is meant to compile any sfml 2.6 code we might have.
# sfml 2.6 code will be phased out eventually.
# this make file was written with chatGPT assistance
######################################################################
# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Target executable name
TARGET = testProgram

# Source files
SRCS = sfml_2_6_1_demo.cpp Particle.cpp

# Object files (automatically replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -lsfml-graphics -lsfml-window -lsfml-system

# Compile each .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
spotless:
	rm -f $(OBJS) $(TARGET)