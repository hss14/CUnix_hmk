#include <stdio.h>
#include <stdlib.h>
#include "fraction_head.h"

int main(int argc, const char *argv[])
{
	float an, ad, bn, bd;
	double result;
	char method;
	fraction a, b, r;
	fraction (*f)();

	if (argc == 5)
	// myfraction 12 / 8 =
	{
		an = atoi( argv[1] );
		ad = atoi( argv[3] );
		a = construct( an, ad);
		result = convert(a);
		print_fraction(a);
		printf(" = %f\n\n", result);
		return 0;
	}
	
	if (argc == 9)
	// myfraction 12 / 8 + 34 / 9 =
	{
		an = atoi( argv[1] );
		ad = atoi( argv[3] );
		bn = atoi( argv[5] );
		bd = atoi( argv[7] );
		method = argv[4][0];
		
		switch(method)
		{
			case '+':	f = add;	break;
			case '-':	f = sub;	break;
			case 'x':	f = multiply;	break;
			case '/':	f = division;	break;
			default:	
			{
				printf("\nwrong arithmetic function symbol!\n");
				printf("example for 9 arguments: myfunction 12 / 8 + 34 / 9  =\n");
				printf("    // the  5th argument should only be '+', '-', 'x', or '/' \n");
				printf("now try again...\n");
				return -1;
			}
		}
		
		a = construct(an, ad);
		b = construct(bn, bd);
		r = f(a,b);
		result = convert(r);
		print_fraction(a);
		printf(" %c ", method);
		print_fraction(b);
		printf(" = ");
		print_fraction(r);
		printf("   = %f\n\n", result);
		return 0;
	}

	printf("\nwrong number of arguments! should be 5 or 9 arguments...\n");
	printf("example for 5 arguments: myfunction 12 / 8 =\n");
	printf("    // to simplify a fraction and convert to double\n");
	printf("example for 9 arguments: myfunction 12 / 8 + 34 / 9  =\n");
	printf("    // to caculate the four arithmetic functions of 2 fractions\n");
	printf("    // the  5th argument should be '+', '-', 'x', or '/' \n");
	printf("now try again...\n");
	return -2;
	
}
