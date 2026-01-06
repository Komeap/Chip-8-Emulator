CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -Llib/x86_64
LIBS = -lprovided -lSDL2 -lm -lSDL2_ttf

SRC = src/main.c src/memory.c src/processor.c
OBJ = $(SRC:.c=.o)
TARGET = chip8
all: $(TARGET)
$(TARGET) : $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJ) $(TARGET)
