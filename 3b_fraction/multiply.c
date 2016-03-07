#include <math.h>
#include "fraction_head.h"

fraction multiply ( fraction x, fraction y)
{
	float n,d;
	fraction result;

	n = x.numerator * y. numerator;
	if ( isfinite(n) )
	{	
		d = x.denominator * y.denominator;
		return construct(n, d);
	}
	
	result.numerator = n;
	result.denominator = 1;
	return result;		
}
