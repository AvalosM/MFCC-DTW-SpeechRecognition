CC      := gcc
OPT     :=
CFLAGS  := -Wall -Wextra -pedantic -Wdouble-promotion -fPIC $(OPT)
AS      := nasm
ASFLAGS := -f elf64 -F DWARF -Wall
LIBS    := -lsndfile -lm

SRC_DIR   := ./src
BUILD_DIR := ./build

AVX := false
ifeq ($(AVX), true)
	LIB_NAME := speechr-avx$(OPT)
	CFLAGS += -D__VECTOR_AVX__
else 
	LIB_NAME := speechr$(OPT)
endif

LIB  := $(BUILD_DIR)/lib$(LIB_NAME).so
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
$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c c_msg
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Assemble
$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm as_msg
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@

# Compile Dynamic Library
$(LIB): $(C_OBJS) $(ASM_OBJS)
	@mkdir -p $(dir $@)
	@echo Compiling $(LIB)...
	@$(CC) $(CFLAGS) -shared $^ $(LIBS) -o $@
	@echo Done!

# Test link
$(TEST): $(C_TEST_SRCS) $(LIB)
	@echo Linking Test executable...
	@$(CC) $(CFLAGS) -o $@ $^ -L./build -l$(LIB_NAME) $(LIBS)
	@echo Done!


.PHONY: lib test install clean c_msg as_msg
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

c_msg:
	@echo Compiling...
	@echo $(CC) flags are: $(CFLAGS)

as_msg:
	@echo $(AS) flags are: $(ASFLAGS)
