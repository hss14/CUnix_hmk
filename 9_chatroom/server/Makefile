CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -g #-DHSSDEBUG

all: server

server: $(OBJS)
	$(CC) $(CFALGS) -o $@ $^

%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean: $(OBJS) server  $(wildcard *.log) 
	rm $^

.PHONY: test
test:
	# just follow the instructions after server launched
	#
	# for detailed information of all previous tests, check "./chatroom.log"
	# be careful
	# "make clean" will rm the log (because it will be a new server after a new make)
	# 
	#
	# ===============================================================================
	./server 
