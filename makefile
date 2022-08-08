CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -Wdouble-promotion -fPIC
AS      := nasm
ASFLAGS := -f elf64 -F DWARF -Wall
LIBS    := -lsndfile -lm

SRC_DIR   := ./src
BUILD_DIR := ./build

LIB  := $(BUILD_DIR)/libspeechr.so
TEST := $(BUILD_DIR)/test

# Source files
C_SRCS      := $(wildcard $(SRC_DIR)/*.c)
C_TEST_SRCS := $(wildcard $(SRC_DIR)/testing/*.c)
ASM_SRCS := $(wildcard $(SRC_DIR)/*.asm)

# Header files
C_HEADERS := $(wildcard $(SRC_DIR)/*.h)

# Object Files
C_OBJS      := $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.c.o)
ASM_OBJS    := $(ASM_SRCS:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.asm.o)

# Compile C
$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble
$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# Compile Dynamic Library
$(LIB): $(C_OBJS) $(ASM_OBJS)
	@mkdir -p $(dir $@)	
	$(CC) $(CFLAGS) -shared $^ $(LIBS) -o $@

# Test link
$(TEST): $(C_TEST_SRCS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ -L./build -lspeechr $(LIBS)


.PHONY: lib test install clean
.DEFAULT_GOAL := lib

lib: $(LIB)

install: lib
	@cp $(LIB) ./notebooks/

test: CFLAGS += -g
test: ASFLAGS += -g
test: $(TEST)

# Remove build directory and executables
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(LIB)
	rm -f $(TEST)