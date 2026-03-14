CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS ?=
SRC := $(wildcard src/*.c)
TARGET := quiz-game

ifeq ($(OS),Windows_NT)
    TARGET := quiz-game.exe
endif

.PHONY: all run clean format

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f quiz-game quiz-game.exe

format:
	clang-format -i src/*.c include/*.h
