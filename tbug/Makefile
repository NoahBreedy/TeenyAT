CC := gcc

SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin

TARGET := $(BIN_DIR)/main

CFLAGS  := -Iinclude
LDFLAGS := -lm
DEBUG_FLAGS :=

EXE_NAME  := tbug

# Platform-specific libraries
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lopengl32 -lgdi32
    RM_CMD := rmdir /s /q
else
    OS := $(shell uname -s)

    ifeq ($(OS),Darwin)
        LDFLAGS += -framework OpenGL -framework Cocoa
    else ifeq ($(OS),Linux)
        LDFLAGS += -lGLU -lGL -lX11
    endif

    RM_CMD := rm -rf
endif

# ------------------------------------------------------------------
# Recursively find all source files
# ------------------------------------------------------------------

SRCS := $(shell find $(SRC_DIR) -name '*.c')

# Convert:
# src/foo.c
# src/gui/window.c
#
# into:
# build/foo.o
# build/gui/window.o
#
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# ------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------

.PHONY: all debug clean

all: $(TARGET)

debug: DEBUG_FLAGS += -fsanitize=address -g -O0
debug: all

# ------------------------------------------------------------------
# Linking
# ------------------------------------------------------------------

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(DEBUG_FLAGS) $^ -o $(BIN_DIR)/$(EXE_NAME) $(LDFLAGS)

# ------------------------------------------------------------------
# Compilation
# ------------------------------------------------------------------

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ------------------------------------------------------------------
# Cleanup
# ------------------------------------------------------------------

clean:
ifeq ($(OS),Windows_NT)
	-if exist $(BUILD_DIR) $(RM_CMD) $(BUILD_DIR)
	-if exist $(BIN_DIR) $(RM_CMD) $(BIN_DIR)
else
	$(RM_CMD) $(BUILD_DIR) $(BIN_DIR)
endif

# ------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------

-include $(DEPS)
