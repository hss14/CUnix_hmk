#include <math.h>
#include "fraction_head.h"

fraction multiply ( fraction x, fraction y)
{
	int n = x.numerator * y. numerator;
	int d = x.denomirator * y.denomirator;
	return construct(n, d);	
}
