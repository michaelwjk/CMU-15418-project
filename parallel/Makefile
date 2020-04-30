DEBUG=0
CC=gcc
OMP=-fopenmp -DOMP
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG)
LDFLAGS= -lm

all: parallel optimized

CFILES = parallel.c ../graph/graph.c ../timer/cycletimer.c
HFILES = ../graph/graph.h ../timer/cycletimer.h
CFILESOPT = parallel_optimized.c ../graph/graph.c ../timer/cycletimer.c

parallel: $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) $(OMP) -o parallel $(CFILES) $(LDFLAGS)

optimized: $(CFILESOPT) $(HFILES)
	$(CC) $(CFLAGS) $(OMP) -o optimized $(CFILESOPT) $(LDFLAGS)