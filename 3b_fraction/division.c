#include <math.h>
#include "fraction_head.h"

fraction division ( fraction x, fraction y)
{
	float n,d;
	fraction result;

	n = x.numerator * y.denominator;
	d = x.denominator * y.numerator;
	if ( isfinite(n) && isfinite(d) )
		return construct(n, d);	
	result.numerator = n/d;
	result.denominator = 1;
	return result;
}

