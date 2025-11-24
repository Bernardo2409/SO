CC = gcc
CFLAGS = -Wall

progs = processador 1_basico 2_argumentos 3_multiplos
objs = $(progs:%=%.o)

.PHONY:     all clean cleanall

all:            $(progs)

$(progs):       %:      %.o
	$(CC) -o $@ $<

$(objs):        %.o:    %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *~ *.o

cleanall:       clean
	rm -f $(progs)