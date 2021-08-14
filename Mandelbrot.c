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
	ComplexNumber* z = newComplexNumber(0.0, 0.0);
	double abs=0.0;

  	// Compute M(Z, C) = Z^2 + C repeatedly
	for(u_int64_t i=1;i <= maxiters;i++) {
			// to avoid memory leaks, allocate everything separately and then free them
			ComplexNumber* p = ComplexProduct(z, z);
			freeComplexNumber(z);
			z = ComplexSum(p, point);
			freeComplexNumber(p);

			abs = ComplexAbs(z);
			if(abs >= threshold) {
				freeComplexNumber(z);
				return i;
			}
	}
	freeComplexNumber(z);

	return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
	double res=(double)resolution;
	double unit_step=scale/(res);
	double r=Re(center), i=Im(center);
	u_int64_t out_idx=0;
	double a,b;
	ComplexNumber* C;
	
	for(u_int64_t col=0;col<(2*resolution + 1);col++) {
		a = i + ((((double)resolution) - ((double)col)) * unit_step);
		for(u_int64_t row=0;row<(2*resolution + 1);row++) {
			b = r - ((((double)resolution) - ((double)row)) * unit_step);
			C = newComplexNumber(b, a);
			*(output + out_idx) = MandelbrotIterations(max_iterations, C, threshold);
			freeComplexNumber(C);
			out_idx++;
		}
	}

}
