CC = clang++
# CFLAGS = -Wall -Werror -Wextra -std=c++2a -g $(shell sdl2-config --cflags)
CFLAGS = -O3 -w -std=c++2a $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
TARGET = chip8
BUILD_DIR = build
SRCS = src/main.cpp src/sdl_manager.cpp src/chip8.cpp
OBJS = $(SRCS:src/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile
$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	@echo "Cleaning up: $(OBJS) $(TARGET)"
	rm -f $(OBJS) $(TARGET)
	@rmdir $(BUILD_DIR) 2>/dev/null || true

# Phony targets
.PHONY: all clean
