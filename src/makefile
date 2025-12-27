includedir = ../include
headerdirs = $(includedir)/
headers := $(shell find $(headerdirs) -name "*.h")
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
CC = cc

include ../common.mk
LDADDCFLAGS = -lpthread -lm -lz

simplekv: $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $@ $(LDADDCFLAGS)

%.o: %.c y.tab.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

y.tab.c: input.l input.y
	flex input.l
	bison -d -y input.y

.PHONY: clean all

clean:
	rm -f *.o simplekv
