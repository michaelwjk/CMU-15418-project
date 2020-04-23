DEBUG=0
CC=gcc
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG)
LDFLAGS= -lm

SOLUTION = parallel

CFILES = first_fit.c ../graph/graph.c ../timer/cycletimer.c
HFILES = ../graph/graph.h ../timer/cycletimer.h

seq: $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) -o parallel $(CFILES) $(LDFLAGS)