CC = gcc

CFLAGS = -Wall

LDFLAGS = -lm

.PHONY: all clean cleanall

all: reg-cri

reg-cri: sharedMemory.o reg-cri.o semaphore.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
semaphore.o: semaphore.c semaphore.h

reg-cri.o:	sharedMemory.h


clean:
	rm -f *.o *~

cleanall:	clean
	rm -f reg-cri

