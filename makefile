CC = gcc
CFLAGS = -Wall -Wextra

ifeq ($(MAKECMDGOALS),debug)
    CFLAGS += -g
endif

TARGET = raytracer
SRC = main.c raytracer.c bmp.c geometry.c vector.c obj.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $(OBJ) -lm

%.o: %.c bmp.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(TARGET) $(OBJ)

.PHONY: all clean debug

debug: all
