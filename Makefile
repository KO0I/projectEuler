CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRC = support/c/fib.c solutions/c/even_fibonacci_numbers_2.c
OBJ = $(patsubst %.c,build/%.o,$(SRC))
TARGET = build/even_fibonacci_numbers_2
.PHONY: all run clean tags
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)
build/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
run: $(TARGET)
	./$(TARGET)
tags: $(SRC)
	ctags $^
clean:
	rm -f $(TARGET) $(OBJ)
