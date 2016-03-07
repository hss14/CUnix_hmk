#include <math.h>
#include "fraction_head.h"

fraction add ( fraction x, fraction y)
{
	float n, d;
	fraction result;

	n = x.numerator * y.denominator + x.denominator * y. numerator;
	if ( isfinite(n) )
	{
		d = x.denominator * y.denominator;
		return construct(n, d);	
	}
	result.numerator = n;
	result.denominator = 1;
	return result;
}
