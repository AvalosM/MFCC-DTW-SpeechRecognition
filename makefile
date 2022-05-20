CC	   := gcc
CFLAGS := -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion
AS		:= nasm
ASFLAGS	:= -f elf64 -F DWARF -Wall
LFLAGS := -lsndfile -lm -no-pie

BUILD_DIR := ./build
SRC_DIR   := ./src
TEST_DIR  := ./src/testing
BENCH_DIR := ./src/benchmark

TARGET := $(BUILD_DIR)/speechr
TEST   := $(BUILD_DIR)/testing/test
BENCH  := $(BUILD_DIR)/benchmark/bench

# Find all the source files needed to build
SRCS       := $(shell find $(SRC_DIR) -maxdepth 1 ! -name "main.c" -name "*.c" -or -name "*.asm")

TEST_SRCS  := $(shell find $(TEST_DIR) -name *.c)

BENCH_SRCS := $(shell find $(BENCH_DIR) -name *.c)

# Object files
OBJS	   := $(SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)
TEST_OBJS  := $(TEST_SRCS:./src/testing/%.c=./build/testing/%.c.o)
BENCH_OBJS := $(BENCH_SRCS:./src/benchmark/%.c=./build/benchmark/%.c.o)

#----------------TARGET LINK------------------#
$(TARGET): $(OBJS) $(BUILD_DIR)/main.c.o
	$(CC) $(CFLAGS) $(OBJS) $(BUILD_DIR)/main.c.o $(LIBS) $(LFLAGS) -o $@

#----------------TEST LINK--------------------#
# Link
$(TEST): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) $(LIBS) $(LFLAGS) -o $@

#----------------BENCHMARK LINK---------------#
$(BENCH): $(BENCH_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(BENCH_OBJS) $(OBJS) $(LIBS) $(LFLAGS) -o $@

# Compile C
$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble
$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: all clean test bench

.DEFAULT_GOAL := all

all: $(TARGET)

test: CFLAGS += -g
test: ASFLAGS += -g
test: $(TEST)

bench: CFLAGS += -g
bench: ASFLAGS += -g
bench: $(BENCH)

# Remove build directory and executables
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	rm -f $(TEST)
	rm -f $(BENCH)