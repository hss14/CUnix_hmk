#include <stdio.h>
#include <stdarg.h>
#include "2b_head.h"

// variadic for load array
void LoadBalls( int count,int *a, ...) 
{
	va_list list;
	int i;
	va_start(list, a);
	for (i=0; i<count; i++)
		a[i] = va_arg(list, int);
	va_end(list);
}
