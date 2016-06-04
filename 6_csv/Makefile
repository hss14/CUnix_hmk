CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -g  #-DHSSDEBUG

all: csv6

csv6: $(OBJS)
	$(CC) $(CFALGS) -o $@ $^

%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean: $(OBJS) csv6
	rm $^

.PHONY: test
test:
	# example of legal augments:
	#
	# ========== transform contents of "name_of_csv_file" to "name_of_bin_file" ==========
	# 	./csv6 t name_of_csv_file name_of_bin_file     
	./csv6 t test.csv test.bin 2>test.err
	# 	for more information:
	# 	"test.csv": original csv file
	# 	"test.bin": tranformed bin file
	# 	"test.csv.bad": broken records of csv file "test.csv"
	# 	"test.err": record the dealing with the broken records or any other errors occured during process
	#
	#
	# ============ print out contents of "name_of_bin_file" ============
	# 	./csv6 p name_of_bin_file 
	./csv6 p test.bin
