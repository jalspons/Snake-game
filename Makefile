# OBJS specifies which files to compile as part of the project
SRC = snake.c

# CC specifies which compiler we're using
CC = gcc

# COMPILER_FLAGS specifies the additinal compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -Wall -Werror `sdl2-config --cflags`

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = `sdl2-config --libs` -lSDL2_ttf

# OBJ_NAME specifies the name of our executable
TARGET = snake

# This is the target that compiles our executable
all : $(SRC)
	$(CC) $(SRC) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)

clean :
	rm -rf $(TARGET)