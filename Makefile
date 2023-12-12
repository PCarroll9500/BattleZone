# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -std=c++11

# Libraries
LIBS = -lGLEW -lGL -lGLU -lglfw

# Source files
SRC = main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Executable name
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)