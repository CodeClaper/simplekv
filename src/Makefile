includedir = ../include
headerdirs = $(includedir)/
headers := $(shell find $(headerdirs) -name "*.h")
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
CC = cc

include ../common.mk
LDADDCFLAGS = -lpthread -lm -lz

simplekv: $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $@ $(LDADDCFLAGS)

%.o: %.c lex.yy.c y.tab.c $(headers)
	$(CC) -c $(CFLAGS) $< -o $@

lex.yy.c: input.l
	flex input.l -o $@

y.tab.c: input.y
	bison -d -y input.y -o $@

.PHONY: clean all

clean:
	rm -f *.o simplekv
