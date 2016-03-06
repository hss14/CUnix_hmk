#include <math.h>
#include "fraction_head.h"

fraction sub ( fraction x, fraction y)
{
	int n = x.numerator * y.denomirator - x.denomirator * y. numerator;
	int d = x.denomirator * y.denomirator;
	return construct(n, d);	
}
