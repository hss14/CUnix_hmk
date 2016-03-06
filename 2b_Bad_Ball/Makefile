CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -g

all: BadBall

BadBall : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean: $(OBJS) BadBall
	rm $^

.PHONY: test
test:
	# 
	# test 1	
	./BadBall 4  -2
	# 
	# test 2
	./BadBall 7  1
	# 
	# test 3
	./BadBall 9.76  3.4
	# 
	# test 4
	./BadBall 12 -1.111
	# 
	# test 5
	-./BadBall 9.8 0
	# 
	# test 6
	-./BadBall 16  9
	#
	# test 7
	-./BadBall 4
