CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -g -DHSSDEBUG

all: client

client: $(OBJS)
	$(CC) $(CFALGS) -o $@ $^

%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean: $(OBJS) client   
	rm $^

.PHONY: test
test:
	# just follow the instructions after client lanched
	# may fail to connect the server if the server is not launched
	#
	#
	# ===============================================================================================
	./client 
