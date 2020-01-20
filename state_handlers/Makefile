# Program name
EXECUTABLE := program

SOURCES := main.c matrix_util.c task_handler.c

# Put all the *.o files in ./build
BUILD_DIR := build
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Specify that our source files are in ./source
SRC_DIR := source
SRC := $(patsubst %.c,source/%.c,$(SOURCES))

CC := gcc
CFLAGS := -O0 -g3 -Werror
# Flags set for zero optimization and detailed debug

.DEFAULT_GOAL := $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(OBJ) -o $@

$(BUILD_DIR) :
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c | $(BUILD_DIR)			
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(.DEFAULT_GOAL) $(BUILD_DIR)






# NOTATER :
# -c compile, -o link
# setup:
# target : dependencies
#	commands
#
# $() for insert var
#	enten rekursiv eller direkte
# 
# Automatic variables:
# $< = første dependency
# $^ = alle dependencies
# $@ = målet vi ønsker å bygge til
#
#
# Ex: 
# kompilering
# main.o : source/main.c
# 	gcc -c $< -o $@
# 
# linking
# out : deps
#  $(CC) $(CFLAGS) $^ -o $@
#
#
# Infererte regler:
# % = mønster, ex main.o <-> main.c (% byttes ut med main)
# Ex:
# %.o : source/%.c
# 	gcc -c $< -o $@
# tillater oss å bruke "make <rule>" (ex make main.o) for alle .c i source
#
# Ex2:
# SOURCES := main.c aaa.c bbb.c ....
# SRC := $(patsubst %.c,source/%.c,$(SOURCES))  // path substitute *.c->./source/*.c				syntax: patsubst from,to,data
# OBJ := $(patsubst %.c,%.o,$(SOURCES))  		// path substitute *.c->*.o for alt i SOURCES
#
# Oppretting av build dir for alle compiler artifacts ex .o filer
#
# SRC_DIR := source
# BUILD_DIR = build
# DEBUG_DIr = debug
# 
# Opprett ./build
# $(BUILD_DIR) :
#	mkdir -p $@			// -p parent directory
#
# Ex:
# $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c $(BUILD_DIR)			// build dir er dep fordi vi må lage ./build hvis den ikke eksisterer
#	gcc -c $< -o $@
# 
# 
#
#
#
#
#
#
#
#
#
#
#
#
