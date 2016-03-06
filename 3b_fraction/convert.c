#include <math.h>
#include "fraction_head.h"

double convert ( fraction x )
{
	return double(x.numerator) / double(x.denominator);	
}
