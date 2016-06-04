#include "csv6.h"

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
