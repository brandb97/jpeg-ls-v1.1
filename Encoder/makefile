DEFINES =
JFLAGS = -O -g
DEFS = globals.h prototypes.h read_write.h marker.h
BASELINEE =  encoder.o marker.o general.o regular_mode.o run_mode.o

.c.o:
	gcc $(JFLAGS) $(DEFINES) -c $*.c 

clean:
	rm *.o jpeglse

jpeglse: $(BASELINEE) 
	gcc $(DEFINES) $(JFLAGS) $(BASELINEE) -lm -o jpeglse

encoder.o:      encoder.c $(DEFS) read_write.h globals.h prototypes.h marker.h
marker.o:       marker.c $(DEFS) marker.h globals.h prototypes.h
general.o:      general.c $(DEFS) globals.h prototypes.h
regular_mode.o: regular_mode.c $(DEFS) globals.h prototypes.h
run_mode.o:     run_mode.c $(DEFS) globals.h prototypes.h
