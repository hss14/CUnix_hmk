#ifndef __CSV_HMK6_H__
#define __CSV_HMK6_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MaxLine 512
#define MaxName 255

#pragma pack(1)

struct StuRecord {
	uint8_t dpt[9];     // 8 characters at most: abbreviation of department
	uint8_t age;
	uint8_t stuid[5];
	uint8_t lenName;
};


uint8_t convert( char num1, char num2 );
int reverse(uint8_t num, int index);

int csv2bin( const char *filecsv, const char *filebin );

int pr_bin(const char *filebin);


#endif
