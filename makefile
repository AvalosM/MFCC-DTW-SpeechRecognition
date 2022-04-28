CC	   := gcc
CFLAGS := -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion
LIBS   := -lsndfile -lm
LFLAGS := -no-pie

AS		:= nasm
ASFLAGS	:= -f elf64 -F DWARF -Wall

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
$(TARGET): msg_building $(OBJS) $(BUILD_DIR)/main.c.o msg_linking
	$(CC) $(CFLAGS) $(OBJS) $(BUILD_DIR)/main.c.o $(LIBS) $(LFLAGS) -o $@

#----------------TEST LINK--------------------#
# Link
$(TEST): msg_building msg_test $(TEST_OBJS) $(OBJS) msg_linking
	$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) $(LIBS) $(LFLAGS) -o $@

#----------------BENCHMARK LINK---------------#
$(BENCH): msg_building msg_bench $(BENCH_OBJS) $(OBJS) msg_linking
	$(CC) $(CFLAGS) $(BENCH_OBJS) $(OBJS) $(LIBS) $(LFLAGS) -o $@

# Compile C
$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble
$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: all clean test bench msg_building msg_test msg_bench msg_linking

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

msg_building:
	@echo Building...
	@echo Sources are: $(SRCS)

msg_test:
	@echo Test sources are: $(TEST_SRCS)

msg_bench:
	@echo Benchmark sources are: $(BENCH_SRCS)

msg_linking:
	@echo Linking...