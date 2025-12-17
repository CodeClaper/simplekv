includedir = ../include
includedir = ../include
headerdirs = $(includedir)/
headers := $(shell find $(headerdirs) -name "*.h")
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
CC = gcc

include ../common.mk

simplekv: $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $@ $(LDADDCFLAGS)

%.o: %.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean all

clean:
	rm -f *.o simplekv
