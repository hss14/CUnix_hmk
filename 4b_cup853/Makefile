CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -g #-DHSSDEBUG

all: cup4

cup4: $(OBJS)
	$(CC) $(CFALGS) -o $@ $^

%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean: $(OBJS) cup4
	rm $^

.PHONY: test
test:
	./cup4 
