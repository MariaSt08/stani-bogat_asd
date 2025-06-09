CC = cc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =
BUILD_DIR = build
SRC_DIR = src

OBJECTS = $(BUILD_DIR)/main.o \
          $(BUILD_DIR)/menu.o \
          $(BUILD_DIR)/quiz.o

EXECUTABLE = $(BUILD_DIR)/stani_bogat

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c \
                     $(SRC_DIR)/menu/menu.h \
                     $(SRC_DIR)/quiz/quiz.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $(SRC_DIR)/main.c -o $@

$(BUILD_DIR)/menu.o: $(SRC_DIR)/menu/menu.c \
                     $(SRC_DIR)/menu/menu.h \
                     $(SRC_DIR)/quiz/quiz.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $(SRC_DIR)/menu/menu.c -o $@

$(BUILD_DIR)/quiz.o: $(SRC_DIR)/quiz/quiz.c \
                     $(SRC_DIR)/quiz/quiz.h
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $(SRC_DIR)/quiz/quiz.c -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean