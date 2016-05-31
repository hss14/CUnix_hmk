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
	# example of legal augments:
	#
	# =======  function A as demanded =======
	# ./freport PATHNAME     
	# ===> ls all files in PATHNAME and whether has the following specific features:
	# 	* "long time no access" if not accessed in the last 30 days
	# 	* "huge file" if size beyond 1M bytes
	# 	* "accessed during specific period" if accessed during the week before last
	# 	* "being read" if is being read by other users exactly when it is traversed by this program
	#
	# =======  function B as demanded =======
	# ./freport --accessed 3 PATHNAME  
	# ===> ls files in PATHNAME that have been accessed with 3 days
	#
	# ./freport --size-beyond 1000000 PATHNAME  
	# ===> ls files in PATHNAME whose size is bigger than 1MB
	#
	# ======= function C as demanded =======
	# ./freport --users-nologin 3 
	# ===> list users who has never logged in in the last 3 days

