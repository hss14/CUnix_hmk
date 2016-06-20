CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o)
CC = gcc
CFLAGS = -Wall -g #-DHSSDEBUG

all: csv8

csv8: $(OBJS)
	$(CC) $(CFALGS) -o $@ $^

%.o : %.c 
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean: $(OBJS) csv8 $(wildcard *.err) $(wildcard *.long) $(wildcard *.bin)  
	rm $^

.PHONY: clean-test	
# ========== remove all the result files remained from the last test
clean-intermediate: $(wildcard *.err) $(wildcard *.long) $(wildcard *.bin)  
	rm $^

.PHONY: test
test:
	# example of legal augments:
	#
	#
	#
	# ================ transform contents of "name_of_csv_file" to "name_of_bin_file" ===============
	# 	./csv8 t name_of_csv_file name_of_bin_file     
	make clean-test
	./csv8 t test.csv test.bin 
	# 	
	# for more information and details, check:
	# 	"test.csv": 
	# 		==> the original csv file
	# 	"test.bin": 
	# 		==> the final tranformed bin file merged by parent process
	# 	"test.csv.long": 
	# 		==> records(line) too long of csv file "test.csv", all discarded for result
	# 	"test.csv.err": 
	# 		==> illegal records, some discarded while others truncated; also log all the other errors occured
	#
	#
	#
	# ============ print out contents of each bin file: "name_of_bin_file"(s) ============
	# 	./csv8 p name_of_bin_file(s) 
	./csv8 p test.bin 
