#include <stdio.h>
#include "fraction_head.h"

void print_fraction ( fraction x)
{
	printf("%.0f/%.0f", x.numerator, x.denominator);
	return;
}
