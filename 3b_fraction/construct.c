#include <math.h>
#include "fraction_head.h"

fraction construct ( int n, int d)
{
	fraction result;
	if (!d)
	{
		result.numerator = 1;
		result.denominator = 0;
		return result;
	}
	if (!n)
	{
		result.numerator = 0;
		result.denominator = 1;
		return result;
	}
	if (n*d < 0)
		result.numerator = -abs(n);
	else
		result.numerator = abs(n);
	result.denominator = abs(d);
	return result;
}
