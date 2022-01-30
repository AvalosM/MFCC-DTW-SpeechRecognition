TARGET	:= speechr
TEST	:= test

CC		:= gcc
CFLAGS	:= -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion
LIBS	:= -lsndfile -lm

AS		:= nasm
ASFLAGS	:= -f elf64 -F DWARF -Wall

BUILD_DIR := ./build
SRC_DIR   := ./src
TEST_DIR  := ./src/testing

# Find all the source files we want to compile
SRCS      := $(shell find $(SRC_DIR) -maxdepth 1 ! -name "main.c" -name *.c -or -name *.asm)

TEST_SRCS := $(shell find $(TEST_DIR) -name *.c)


# Object files
OBJS	  := $(SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:./src/testing/%.c=./build/testing/%.c.o)

#----------------TARGET LINK----------------#
$(TARGET): $(OBJS) $(BUILD_DIR)/main.c.o msg_linking
	$(CC) $(CFLAGS) $(OBJS) $(BUILD_DIR)/main.c.o -no-pie $(LIBS) -o $@

#----------------TEST LINK----------------#
# Link
$(TEST): $(TEST_OBJS) $(OBJS) msg_linking
	$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) $(LIBS) -no-pie -o $@

# Compile C
$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c msg_compiling
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble .asm
$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: all debug clean test

.DEFAULT_GOAL := all

all: $(TARGET)

test: CFLAGS += -g
test: ASFLAGS += -g

# Debug compile target
debug: CFLAGS += -g
debug: ASFLAGS += -g
debug: all

# Remove build directory and executables
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	rm -f $(TEST)

.INTERMEDIATE: msg_compiling msg_linking
msg_compiling:
	@echo Compiling...

msg_linking:
	@echo Linking...
