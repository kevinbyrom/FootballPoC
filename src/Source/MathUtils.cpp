#include <windows.h>
#include <math.h>
#include "MathUtils.h"



int Round(float val)
	{

	////////////////////////////
	//						  //
	// Used to round a number //
	//						  //
	////////////////////////////


	return (val - (float)(int)val >= 0.5f ? (int)val + 1 : (int)val);
	}




int QuadraticFormula(float a, float b, float c, float * x1, float * x2)
	{

	//////////////////////////////////////////////////////
	//													//
	// Used to perform the quadratic formula			//
	//													//
	// x = -b +- sqrt(b^2 - 4ac)  = -b +- sqrt(section) //
	//	   _____________________	___________________	//
	//	          2a						2a	        //
	//													//
	//////////////////////////////////////////////////////


	float section = 0.0f;
	float sqrtsection = 0.0f;
	

	// Validate the x1 and x2 pointers 

	if (x1 == NULL || x2 == NULL || a == 0)
		return 0;


	// Calculate the section

	section = (b * b) - (4 * a * c);


	// Return zero if the section is negative (cannot find sqrt of negative)

	if (section < 0)
		return 0;


	// Calculate the square root of the section

	sqrtsection = sqrtf(section);


	// Figure out the values of x1 and x2

	*x1 = (-b + sqrtsection) / (2 * a);
	*x2 = (-b - sqrtsection) / (2 * a);


	return 1;
	}