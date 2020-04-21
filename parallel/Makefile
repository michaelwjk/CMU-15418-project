DEBUG=0
CC=gcc
OMP=-fopenmp -DOMP
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG)
LDFLAGS= -lm

SOLUTION = parallel

CFILES = parallel.c ../graph/graph.c
HFILES = ../graph/graph.h

parallel: $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) $(OMP) -o parallel $(CFILES) $(LDFLAGS)