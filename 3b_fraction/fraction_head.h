# ifndef __FRACTION_HEAD__
# define __FRACTION_HEAD__

typedef struct fraction {
	float numerator;   //fen zi
	float denominator; //fen mu
}fraction;
// always int indeed, except for NAN and INFINITY

unsigned int myGCD(unsigned int, unsigned int);
// assume int a,b != 0

fraction construct (float, float);
// n, d;

void print_fraction (fraction);

fraction add( fraction, fraction);

fraction sub( fraction, fraction);

fraction multiply( fraction, fraction);

fraction division( fraction, fraction);

double convert ( fraction );


# endif
