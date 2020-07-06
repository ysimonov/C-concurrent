#Makefile for building Pthread programs

TARGETS = lab2part5

CROSS_TOOL = 
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -O0 -pthread -D_GNU_SOURCE #-Werror -std=c99

all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $(CFLAGS) $@.c -lm -o $@

clean:
	rm -f $(TARGETS)
