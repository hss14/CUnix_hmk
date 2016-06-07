#include "csv7.h"

uint8_t convert( char num1, char num2 )
{
	uint8_t r1, r2, result;
	switch(num1)
	{
		case '0':	r1=(uint8_t)0x00;	break;
		case '1':	r1=(uint8_t)0x01;	break;
		case '2':	r1=(uint8_t)0x02;	break;
		case '3':	r1=(uint8_t)0x03;	break;
		case '4':	r1=(uint8_t)0x04;	break;
		case '5':	r1=(uint8_t)0x05;	break;
		case '6':	r1=(uint8_t)0x06;	break;
		case '7':	r1=(uint8_t)0x07;	break;
		case '8':	r1=(uint8_t)0x08;	break;
		case '9':	r1=(uint8_t)0x09;	break;
		default:	fprintf(stderr, "convert: illegal parameter num1 = %c\n", num1); return (uint8_t)0xFF;
	}
	switch(num2)
	{
		case '0':	r2=(uint8_t)0x00;	break;
		case '1':	r2=(uint8_t)0x01;	break;
		case '2':	r2=(uint8_t)0x02;	break;
		case '3':	r2=(uint8_t)0x03;	break;
		case '4':	r2=(uint8_t)0x04;	break;
		case '5':	r2=(uint8_t)0x05;	break;
		case '6':	r2=(uint8_t)0x06;	break;
		case '7':	r2=(uint8_t)0x07;	break;
		case '8':	r2=(uint8_t)0x08;	break;
		case '9':	r2=(uint8_t)0x09;	break;
		default:	fprintf(stderr, "convert: illegal parameter num2 = %c\n", num1); return (uint8_t)0xFF;
	}
	result = (r1<<4) | r2;
	return result;
}

int reverse(uint8_t num, int index)
{
	if( index % 2)   //odd index, get the right part of num
	{
		return (int)(num&0x0F);
	}
	else		 //even index, get the left part of num
	{
		return (int)((num&0xF0) >> 4);
	}
}
