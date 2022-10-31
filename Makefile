CC=gcc
CCOPTS=--std=gnu99 -Wall
AR=ar

OBJS=process.o

HEADERS=process.h

LIBS=libprocess.a

BINS=top


.phony: clean all


all:	$(BINS) $(LIBS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

libprocess.a:	$(OBJS) 
	$(AR) -rcs $@ $^


top:	top.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^

stuff: stuff.c
	$(CC) $(CCOPTS) -o $@ $^
clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
