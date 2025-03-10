CC = clang++
CC_FLAGS = \
	-std=gnu++23 \
	-Wall \
	-Wextra

LD = clang++
LD_FLAGS = \
	-framework Cocoa \
	-framework CoreGraphics

# Supported build types: DEBUG, RELEASE
BUILD_TYPE ?= DEBUG

BUILD_BASE_DIR = build
SRC_DIR = src

ifeq ($(BUILD_TYPE), RELEASE)
	BUILD_DIR = $(BUILD_BASE_DIR)/release
	CC_FLAGS += -DNDEBUG -O2
	LD_FLAGS +=
else ifeq ($(BUILD_TYPE), DEBUG)
	BUILD_DIR = $(BUILD_BASE_DIR)/debug
	CC_FLAGS += -g -O0 -fsanitize=address
	LD_FLAGS += -O0 -fsanitize=address
else
	$(error Unsupported BUILD_TYPE $(BUILD_TYPE))
endif

SRC = ./src/_unity.cpp
OBJ = $(BUILD_DIR)/_unity.o
EXE = $(BUILD_DIR)/omba

.PHONY: all
all: build

.PHONY: build
build:
	@echo "Clean old artifacts..."
	@rm -fr $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)

	@echo "Compiling..."
	$(CC) $(CC_FLAGS) -c -o $(OBJ) $(SRC)

	@echo "Linking..."
	$(CC) $(LD_FLAGS) -o $(EXE) $(OBJ)

.PHONY: help
help:
	@echo "TARGETS"
	@echo ""
	@echo "  <default>      - Build executable"
	@echo "  build          - Build executable"
	@echo "  help           - Prints this help"
	@echo "  verify         - Runs verifications"
	@echo ""

.PHONY: verify
verify:
	@./scripts/verify-format-cpp.sh
	@./scripts/verify-style-cpp.sh
