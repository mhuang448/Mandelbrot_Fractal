/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
	u_int64_t iters = 0;
	ComplexNumber *Z = newComplexNumber(0, 0);
	while (iters < maxiters && ComplexAbs(Z) < threshold) {
		ComplexNumber *ZSquared = ComplexProduct(Z, Z);
		ComplexNumber *newZ = ComplexSum(ZSquared, point);
		//return number of iterations if absolute value of Z is > threshold
		/*
		if (ComplexAbs(Z) > threshold) {
			freeComplexNumber(ZSquared);
			freeComplexNumber(newZ);
			freeComplexNumber(Z);
			return iters + 1;
		}
		*/
		freeComplexNumber(Z);
		Z = newZ;
		freeComplexNumber(ZSquared);
		
		
		iters++;  
	}
	freeComplexNumber(Z);
	if (iters < maxiters) {
		return iters;
	} else {
		return 0;
	}
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
	u_int64_t size = (2 * resolution + 1) * (2 * resolution + 1);
	u_int64_t median = size / 2;
	long double delta = (long double) scale / (long double) resolution;
	long double real = (long double) Re(center) - scale;
	long double imag = (long double) Im(center) + scale; 
	ComplexNumber *number = newComplexNumber(real, imag);
	// make arbitrary *new number
	//ComplexNumber *newNumber = newComplexNumber(0, 0);
	int col = 0;
	
	for (int i = 0; i < size; i++) {
		output[i] = MandelbrotIterations(max_iterations, number, threshold);
		col++;
		
		if (col < (2 * resolution + 1)) {
			//freeComplexNumber(newNumber);
			//newNumber = newComplexNumber(Re(number) + delta, Im(number));
			real +=  delta;
		} else  {
			//freeComplexNumber(newNumber);
			//newNumber = newComplexNumber(Re(center) - scale, Im(number) - delta);
			col = 0;
			real = Re(center) - scale;
			imag -= delta;
		}
		freeComplexNumber(number);
		//number = newNumber;
		//update number with values for next iteration
		number = newComplexNumber(real, imag);

	}
	freeComplexNumber(number);
}


