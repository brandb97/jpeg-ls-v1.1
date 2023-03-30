# Makefile for jpeglsd

# Copyright (C) 1997, University of British Columbia. All rights reserved.

DEFINES =
JFLAGS = -O -g
DEFS = globals.h prototypes.h read_write.h marker.h
BASELINED =  decoder.o marker.o general.o regular_mode.o run_mode.o

.c.o:
	gcc $(JFLAGS) $(DEFINES) -c $*.c 

.c.ln:
	lint -c $*.c 

clean:
	rm *.o jpeglsd

jpeglsd: $(BASELINED) 
	gcc $(DEFINES) $(JFLAGS) $(BASELINED) -lm -o jpeglsd

decoder.o:      decoder.c $(DEFS) read_write.h globals.h prototypes.h marker.h
marker.o:       marker.c $(DEFS) marker.h globals.h prototypes.h
general.o:      general.c $(DEFS) globals.h prototypes.h
regular_mode.o: regular_mode.c $(DEFS) globals.h prototypes.h
run_mode.o:     run_mode.c $(DEFS) globals.h prototypes.h
