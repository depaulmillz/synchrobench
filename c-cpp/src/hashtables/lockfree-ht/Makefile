ROOT = ../../..

include $(ROOT)/common/Makefile.common

ifeq ($(STM),SEQUENTIAL)
  BINS = $(BINDIR)/sequential-hashtable
else ifeq ($(STM),LOCKFREE)
  BINS = $(BINDIR)/lockfree-hashtable
else
  BINS = $(BINDIR)/$(STM)-hashtable
endif

CFLAGS += -std=gnu89

.PHONY:	all clean

all:	main

linkedlist.o:
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/linkedlist.o linkedlist.c

harris.o: linkedlist.h linkedlist.o
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/harris.o harris.c

ll-intset.o: linkedlist.h linkedlist.o harris.o
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/ll-intset.o llintset.c

hashtable.o: linkedlist.h linkedlist.o harris.o ll-intset.o
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/hashtable.o hashtable.c

intset.o: linkedlist.h harris.o hashtable.o 
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/intset.o intset.c

test.o: linkedlist.o harris.o intset.o hashtable.o intset.o
	$(CC) $(CFLAGS) -c -o $(BUILDIR)/test.o test.c

main: linkedlist.o harris.o intset.o hashtable.o intset.o test.o 
	$(CC) $(CFLAGS) $(BUILDIR)/linkedlist.o $(BUILDIR)/harris.o $(BUILDIR)/ll-intset.o $(BUILDIR)/hashtable.o $(BUILDIR)/intset.o $(BUILDIR)/test.o -o $(BINS) $(LDFLAGS)

clean:
	-rm -f $(BINS)
