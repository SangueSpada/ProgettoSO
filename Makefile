CC=gcc
CCOPTS=--std=gnu99 -Wall
AR=ar

OBJS=process.o functions.o

HEADERS=process.h functions.h

LIBS=libtop.a 

BINS=top stuff


.phony: clean all


all:	$(BINS) $(LIBS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

libtop.a:	$(OBJS) 
	$(AR) -rcs $@ $^
	$(RM) $(OBJS)


top:	top.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^

stuff: stuff.c
	$(CC) $(CCOPTS) -o $@ $^
clean:
	rm -rf *.o *~ $(LIBS) $(BINS)
