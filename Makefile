#Makefile for building MPI programs
TARGETS = lab1part5

#C-compiler
CC    =   mpicc

#Add your choice of flags
CFLAGS = -g -O2 -w -std=c99 #-Werror -Wall

all: clean $(TARGETS)
        
$(TARGETS): 
	$(CC) $(CFLAGS) $@.c -lm -o $@

clean:
	rm -f $(TARGETS)

