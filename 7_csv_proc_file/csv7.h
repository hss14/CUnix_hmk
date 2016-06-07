#ifndef __CSV_HMK6_H__
#define __CSV_HMK6_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MaxLine 512
#define MaxName 255
#define SplitSize 5
#define DirectoryTmp "tmp/"

#pragma pack(1)

struct StuRecord {
	uint8_t dpt[9];     // 8 characters at most: abbreviation of department
	uint8_t age;
	uint8_t stuid[5];
	uint8_t lenName;
};

struct pidlist {
	pid_t child;
	struct pidlist *next;
};


uint8_t convert( char num1, char num2 );
int reverse(uint8_t num, int index);

int csv2bin( const char *filecsv, const char *filebin );
int child_csv2bin( const char *csvsplit, const char *binsplit, const char *badrecords);

void addlist( pid_t pid, struct pidlist **head);
pid_t pop( struct pidlist **head);
void printlist( struct pidlist *head);

int pr_bin(const char *filebin);


#endif
