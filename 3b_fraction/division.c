#include <math.h>
#include "fraction_head.h"

fraction division ( fraction x, fraction y)
{
	int n = x.numerator * y.denomirator;
	int d = x.denomirator * y.numerator;
	return construct(n, d);	
}
