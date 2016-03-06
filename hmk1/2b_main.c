#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "2b_head.h"

int main(int argc, const char *argv[] )
{
	int a[12]={0};
	int num, i, x;
	double diff;

	if ( argc < 3 ) {
		printf("\nwrong number of arguments ! \nshould get 3 input arguments:\nexecutable filename(which is BadBall)\nNo.BadBall:int 1~12\n");
		printf("diff: any real number other than zero, diff<0 means the bad ball is lighter, otherwise heavier\n\n");
		return -3;
	}

	num = atoi ( argv[1] );
	diff = atof (argv[2] );
	if ( (num<1)||(num>12) )
	{
		printf("\nwrong argument:num!\nnum should be int among 1~12!\ntry again!\n\n");
		return -2;
	}
	if (!diff)
	{
		printf("\nwrong argument:diff!\ndiff should be any real number other than zero\ndiff<0 means the bad ball is lighter, otherwise heavier\ntry again!\n\n");
		return -1;
	}

	a[num-1] = (diff>0) ? 1 : -1;
	printf("\nShow weight of balls corresponding to sequence number:\n");
	for (i=0; i<12; i++)
		printf("%d(%d) ", (i+1), a[i]);
	printf("\n");
	x = DiffWeight(a);
	printf("\nno.%d is the bad ball:", abs(x));
	if (x>0)
		printf(" Heavier!\n\n");
	else
		printf(" Lighter!\n\n");

	return 0;
}
