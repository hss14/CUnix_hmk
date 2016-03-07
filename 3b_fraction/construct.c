#include <math.h>
#include <stdlib.h>
#include "fraction_head.h"

fraction construct ( float n, float d)
{
	fraction result;
	int symbol;
	unsigned int gcdnumber;
	if (!d)
	{
		result.denominator = 1;
		if (!n)
			result.numerator = NAN;
		else if (n > 0)
			result.numerator = INFINITY;
		else
			result.numerator = -INFINITY;
		return result;
	}

	if (!n)
	{
		result.numerator = 0;
		result.denominator = 1;
		return result;
	}
	
	if(n*d<0)
		symbol = -1;
	else 
		symbol = 1;
	n = abs(n);
	d = abs(d);
	gcdnumber = myGCD( (unsigned int)n, (unsigned int)d );
	n = n/gcdnumber;
	d = d/gcdnumber;
	result.numerator = symbol * n;
	result.denominator = d;
	return result;
}
