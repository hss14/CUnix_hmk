#include "fraction_head.h"
#include <math.h>

// assume a,b != 0
unsigned int myGCD( unsigned int a, unsigned int b)
{
	unsigned int r;
	unsigned int temp;
	if (a<b)
	{
		temp = a;
		a = b;
		b = temp;
	} // now a>=b

	r = a % b;
	while (r)
	{
		a = b;
		b = r;
		r = a % b;
	}
	return b;	
}
