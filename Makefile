CC = clang++
CC_FLAGS = -std=gnu++23 -Wall -Wextra

LD = clang++
LD_FLAGS = -framework Cocoa -framework CoreGraphics

BUILD_TYPE ?= DEBUG

BUILD_BASE_DIR = build
SRC_DIR = src

ifeq ($(BUILD_TYPE), RELEASE)
	BUILD_DIR = $(BUILD_BASE_DIR)/release
	CC_FLAGS += -O2
	LD_FLAGS +=
else ifeq ($(BUILD_TYPE), DEBUG)
	BUILD_DIR = $(BUILD_BASE_DIR)/debug
	CC_FLAGS += -DDEBUG -g -O0
	LD_FLAGS += -O0
else
	$(error Unsupported BUILD_TYPE $(BUILD_TYPE))
endif

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.cpp=.o))

TARGET = $(BUILD_DIR)/omba

.PHONY: all
all: init $(TARGET)

.PHONY: init
init:
	echo "SRCS: $(SRCS)"
	echo "OBJS: $(OBJS)"
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	@echo "Linking executable..."
	$(CC) $(LD_FLAGS) $^ -o "$@"

$(OBJS): $(SRCS)
	@echo "Compiling $< --> $@"
	$(CC) $(CC_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR)

.PHONY: help
help:
	@echo "Print help"

