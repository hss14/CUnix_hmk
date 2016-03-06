# ifndef __FRACTION_HEAD__
# define __FRACTION_HEAD__

typedef struct fraction {
	int numerator;   //fen zi
	int denominator; //fen mu
} fraction;

fraction construct (int, int);
// int n, int d;
//todo: gcd

fraction add( fraction, fraction);

fraction sub( fraction, fraction);

fraction multiply( fraction, fraction);

fraction division( fraction, fraction);
// zero fraction y???

double convert ( fraction );


# endif
