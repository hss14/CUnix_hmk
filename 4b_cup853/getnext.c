#include "cup.h"

void getnext( int abc, int nextstep[] )
{
	int a, b, c;
	a = mygeta(abc);
	b = mygetb(abc);
	c = mygetc(abc);
	if( (b < BROOM) && (a>0) )
		nextstep[0] = get_abc( fromcup(a,b,BROOM), tocup(a,b,BROOM), c );   //A to B
	if( (c < CROOM) && (a>0) )
		nextstep[1] = get_abc( fromcup(a,c,CROOM), b, tocup(a,c,CROOM) );   //A to C
	if( (a < AROOM) && (b>0) )
		nextstep[2] = get_abc( tocup(b,a,AROOM), fromcup(b,a,AROOM), c );   //B to A
	if( (c < CROOM) && (b>0) )
		nextstep[3] = get_abc( a, fromcup(b,c,CROOM), tocup(b,c,CROOM) );   //B to C
	if( (a < AROOM) && (c>0) )
		nextstep[4] = get_abc( tocup(c,a,AROOM), b, fromcup(c,a,AROOM) );   //C to A
	if( (b < BROOM) && (c>0) )
		nextstep[5] = get_abc( a, tocup(c,b,BROOM), fromcup(c,b,BROOM) );   //C to B
#ifdef HSSDEBUG
int i;
printf("%03d = %d %d %d,", abc, a, b, c);
for (i=0; i<6; i++)
	printf(" %03d,", nextstep[i] );
printf("\n");
#endif
	return;
}

int get_abc(int a, int b, int c)
{
	return ( 100 * a + 10 * b + c );
}

int mygeta(int abc)
{
	return ( abc / 100 );
}

int mygetb(int abc)
{
	int a = abc/100;
	return (abc - 100 * a) / 10 ;
}

int mygetc(int abc)
{
	return abc % 10;
}

int fromcup(int a, int b, int broom)
{
	if ( a <= (broom-b) )
		return 0;
	else
		return ( a-(broom-b) );
}

int tocup(int a, int b, int broom)
{
	if ( a <= (broom-b) )
		return a+b;
	else
		return broom;
}
