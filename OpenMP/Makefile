#Makefile for building OpenMP C programs

#root directory
ROOTDIR  = $(shell pwd)

#operational system
OPSYS    = $(shell uname -s )

#source files
SRCS_C   = lab3part5.c 

OBJS_C   = $(SRCS_C:.c=.o)

#targets
TARGET   = lab3part5 

#C-compiler
CC    =   gcc

#for different types of compilers
ifeq ($(CC), gcc)
CFLAGS = -fopenmp
OUTPUTFILE=OUTPUT_lab3part5$(CC)
endif
 
ifeq ($(CC), icc)
CFLAGS = -openmp
OUTPUTFILE=OUTPUT_lab3part5$(CC)
endif

#Add your choice of flags
FFLAGS = -g -w -Ofast -fomit-frame-pointer -march=native -Wall -pedantic     

all: $(TARGET)

$(TARGET): $(OBJS_C)
	$(CC) -o $@ $(CFLAGS) $(OBJS_C)

$(OBJS_C): $(SRCS_C)
	$(CC) $(CFLAGS) $(FFLAGS) -c $(SRCS_C)

#get help for make options
help:; 
	@echo " "
	@echo "Operating System Detected: $(OPSYS) "
	@echo " "
	@echo "USAGE: "
	@echo "make help       To get this listing"
	@echo "make            To compile the OpenMP  program in current environment"
	@echo "make clean      Remove *.o and executable files"
	@echo "make list       List the compilers in current environment"
	@echo "make brun       Submit a batch job"
	@echo " "
 
#list of compilers in a current environment
list:
	@echo
	@echo "OPSYS:       $(OPSYS)"
	@echo "ROOTDIR:     $(ROOTDIR)"
	@echo "FC Compiler: $(FC)"
	@echo "C  Compiler: $(CC)"
	@echo "CFLAGS:      $(CFLAGS)"
	@echo "OUTPUT FILE: $(OUTPUTFILE)"
	@echo "ARCH:        $(ARCH)"
	@echo " "

#clean all files
clean:
	@echo "Removing *.o and executable files"
	rm -rf *.o $(TARGET) OUTPUT_my_OpenMP*

#submit a batch job
brun:;
	rm -rf $(OUTPUTFILE)
	sqsub -r 1h --mpp=1.0G  -o $(OUTPUTFILE) -f threaded -n 8 ./$(TARGET)
