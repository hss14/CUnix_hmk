CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -g -DHSSDEBUG

all: freport

freport: $(OBJS)
	$(CC) $(CFALGS) -o $@ $^

%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean: $(OBJS) freport
	rm $^

.PHONY: test
test:
	#
	# part 1
	#
	./myfraction 66 / 2016 =
	#
	# part 2
	#
	./myfraction 0 / 9 / 7 / 14 =	
	# 
	# part 3: play with INFINITY
	#
	./myfraction -77 / 0 x -1 / 0 =	
	# 
	# part 4: play with and NAN
	#
	./myfraction 4 / 0 + 0 / 0 =	
	#
	# part 5: play with zero in numerator of the divisor
	#
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
