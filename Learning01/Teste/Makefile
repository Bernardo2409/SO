CC = gcc
CFLAGS = -Wall

progs = ex5 ex7
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