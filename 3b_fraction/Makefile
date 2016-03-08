CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -g

all: myfraction

myfraction: $(OBJS)
	$(CC) $(CFALGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean: $(OBJS) myfraction
	rm $^

.PHONY: test
test:
	#
	# part 1
	#
	./myfraction 66 / 2016 =
	./myfraction 34 / -2015 =
	./myfraction 0 / 73 =
	./myfraction -48 / -6 =
	./myfraction -3 / 0 =
	./myfraction 0 / 0 =
	#
	# part 2
	#
	./myfraction 1 / 2 + 1 / 3 =	
	./myfraction 1 / 2 - 1 / 3 =	
	./myfraction 1 / 2 x 1 / 3 =	
	./myfraction 1 / 2 / 1 / 3 =	
	./myfraction 0 / 9 - 7 / 3 =	
	./myfraction 0 / 9 x 7 / 3 =	
	./myfraction 0 / 9 / 7 / 14 =	
	# 
	# part 3: play with INFINITY
	#
	./myfraction 1 / 2 - 1 / 0 =	
	./myfraction 1 / 2 + 1 / 0 =	
	./myfraction 89 / 0 - 91 / 0 =	
	./myfraction -77 / 0 + -1 / 0 =	
	./myfraction -77 / 0 / -1 / 0 =	
	./myfraction -77 / 0 x -1 / 0 =	
	# 
	# part 4: play with and NAN
	#
	./myfraction 0 / 0 x 1 / 3 =	
	./myfraction 4 / 0 + 0 / 0 =	
	#
	# part 5: play with zero in numerator of the divisor
	#
	./myfraction 1 / 2 / 0 / 3 =	
	./myfraction 24 / 198 / 0 / -77 =
	#
	# part 6: test illegal input parameters
	#
	-./myfraction 1 / 2 + 3 / 
	#
	-./myfraction 1 / 2 @ 3 / 4 =
	#
	# end of test
	#
