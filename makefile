CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = raytracer
SRC = raytracer.c bmp.c vector.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c bmp.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(TARGET) $(OBJ)
