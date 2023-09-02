CC = gcc
CFLAGS = -g -O3 -std=c11
TARGET_EXEC = clox

BUILD_DIR = build
SRC_DIR = src

SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(SOURCES:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)