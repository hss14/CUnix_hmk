#include <stdio.h>
#include "2b_head.h"

// a[]: weitht of ball corresponding to sequence number
// left[]/right[]: sequence numbers of balls on the left/right side of the balance
// count: number of balls on each side of the balance

// print the content on the balance and the result, return result as -1,0,or 1

int balance(int *a, int *left, int *right, int count)
{
	int num, i;
	int result = 0;
	char ResultPrint[3] = {'<', '=', '>'};
	printf("\n ( ");
	for (i=0; i<count; i++)
	{
		num = left[i];
		result += a[num];
		printf("%d ", num+1);	
	}
	printf(") || ( ");
	for (i=0; i<count; i++)
	{
		num = right[i];
		result -= a[num];
		printf("%d ", num+1);
	}
	printf(")\nLeft %c Right\n", ResultPrint[result+1]);
	return result;
}
