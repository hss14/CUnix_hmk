#include <stdio.h>
#include "head.h"

// a[12]: weight of ball corresponding to sequence number
// 
// return x: 
// abs(x)= the sequence number of the bad ball
// sign(+ or -) = the bad ball is heavier or lighter

int DiffWeight(int *a)
{
	int left[4], right[4];
	int count;
	int result1, result2, result3;

	//1. (1,2,3,4) || (5,6,7,8)
	count = 4;
	LoadBalls( count, left, 0, 1, 2, 3);
	LoadBalls( count, right, 4, 5, 6, 7); 
	result1 = balance(a, left, right, count);
	
	if (!result1) //balance, bad among (9,10,11,12)
	{
		// 2. (9,10,11) || (1,2,3)
		count = 3;
		LoadBalls( count, left, 8, 9, 10);
		LoadBalls( count, right, 0, 1, 2); 
		result2 = balance(a, left, right, count);

		if (!result2)  //balance, bad == 12
		{
			// 3.  (12) || (1)
			count = 1; left[0] = 11; right[0] = 0;
			result3 = balance(a, left, right, count);
			return (12 * result3);
		}
		else  //bad among(9,10,11), x * result2 >0 
		{
			//3.  (9) || (10)
			count = 1; left[0] = 8; right[0] = 9;
			result3 = balance(a, left, right, count);
			if (!result3)
				return (11 * result2);
			else
				return result2 * (19-result2*result3)/2;
		}
	}

	else
	{
		//2. (1,2,5) || (3,4,6)
		count = 3;
		LoadBalls( count, left, 0, 1, 4);
		LoadBalls( count, right, 2, 3, 5); 
		result2 = balance( a, left, right, count); 
		
		if (!result2)  //balance, bad among { 7, 8}
		{
			//3. (7) || (8)
			count = 1; left[0] = 6; right[0] = 7;
			result3 = balance(a, left, right, count);
			return  -result1 * (15 + result1 * result3)/2;
		}
		
		else if ( result1 * result2 > 0 ) 
		// either {1,2}*result1, or {6}*(- result1) 
		{
			//3. (1) || (2)
			count = 1; left[0] = 0; right[0] = 1;
			result3 = balance(a, left, right, count);
			if (!result3)  //balance
				return -result1 * 6;
			else if (result3 * result1 > 0)
				return result1 * 1;
			else
				return result1 * 2;
		}

		else
		// either {3,4}*result1, or {5}*result2 
		{
			//3. (3) || (4)
			count = 1; left[0] = 2; right[0] = 3;
			result3 = balance(a, left, right, count);
			if (!result3)  //balance
				return result2 * 5;
			else if (result3 * result1 > 0)
				return result1 * 3;
			else
				return result1 * 4;
		}
	}
}




