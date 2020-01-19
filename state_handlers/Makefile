SOURCES := main.c matrix_util.c task_handler.c

BUILD_DIR := build
OBJ := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

SRC_DIR := source
SRC := $(SOURCE:%.c=$(SRC_DIR)/%.c)

CC := gcc
CFLAGS := -O0 -g3 -Werror

.DEFAULT_GOAL := a.out

$(.DEFAULT_GOAL) : $(OBJ)
	$(CC) $(OBJ) -o $@

$(BUILD_DIR) :
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(.DEFAULT_GOAL) $(BUILD_DIR)
