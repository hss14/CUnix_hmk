#include <math.h>
#include "fraction_head.h"

fraction add ( fraction x, fraction y)
{
	int n, d;
	if ( !x.denominator || !y.denominator )
		
	n = x.numerator * y.denomirator + x.denomirator * y. numerator;
	d = x.denomirator * y.denomirator;
	return construct(n, d);	
}
