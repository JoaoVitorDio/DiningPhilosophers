OBJECTS = philosophers.o monitor.o
CLFLAGS = -O0 -pthread

all:	philosophers

run:	philosophers
	./philosophers

philosophers:	$(OBJECTS)
	gcc $(OBJECTS) $(CLFLAGS) -o philosophers

philosophers.o:		philosophers.c
	gcc -c $(CLFLAGS) philosophers.c

monitor.o:	monitor.c monitor.h
	gcc -c $(CLFLAGS) monitor.c

clean:
	rm *.o philosophers