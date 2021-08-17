/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/* Custom Helper Function to write output P6 .ppm files */
int writeP6File(char* colorfile, u_int64_t size, char* outputfile, u_int64_t *output)
{
	int* colorcount;
	int val=0;
	colorcount=&val;
	uint8_t** colors = FileToColorMap(colorfile, colorcount);
	printf("colorcount : %d\n", *colorcount);
	FILE* fp=fopen(outputfile, "w");
	fprintf(fp, "P6 %d %d 255\n", size, size);
	
	// fwrite(&(colors[idx][0]), sizeof(colors[idx][0]), 1, fp);
	// fwrite(&(colors[idx][1]), sizeof(colors[idx][1]), 1, fp);
	// fwrite(&(colors[idx][2]), sizeof(colors[idx][2]), 1, fp);

	u_int64_t iter;
	int idx;
	int zero=0;
	
	for(u_int64_t h=0;h<size;h++) {
		for(u_int64_t w=0;w<size;w++) {
			iter=output[h*size + w];

			if(iter==0) {	
				fprintf(fp, "%c%c%c", 0,0,0);
			}
			else {
				idx=((((int)iter)-1)%(*colorcount));
				fprintf(fp, "%c%c%c", colors[idx][0], colors[idx][1], colors[idx][2]);
			}
			
		}
	}

	int res=fclose(fp);
	for(int i = 0;i < (*colorcount);i++) free(colors[i]);
	free(colors);	

	return 0;
}

/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
	// r=ratio between two consecutive frames
	// scale_n=(r)^(n-1)scale_1
	// logr = (log(scale_N) - log(scale_1))/(N-1)
	double r;
	double t1=log(finalscale) - log(initialscale);
	t1 = t1/((double)framecount - 1.0);
	r = exp(t1);

	for(int i=1;i <= framecount;i++) {
		t1=pow(r,i-1)*initialscale;
		printf("t1 : %0.9lf\n", t1);
		Mandelbrot(threshold, max_iterations, center, t1, resolution, output[i-1]);
	}
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/


  	// printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);


	if (argc != 11) {
		printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
		printUsage(argv);
		return 1;
	}
	double threshold, initialscale, finalscale;
	int framecount;
	ComplexNumber* center;
	u_int64_t max_iterations, resolution;
	char* outputfolder;
	char* colorfile;

	threshold = atof(argv[1]);
	max_iterations = (u_int64_t)atoi(argv[2]);
	center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	initialscale = atoi(argv[5]);
	finalscale = atof(argv[6]);
	framecount = atof(argv[7]);
	resolution = (u_int64_t)atoi(argv[8]);
	outputfolder=argv[9];
	colorfile=argv[10];
	

	if (threshold <= 0 || initialscale <= 0 || finalscale <=0  || max_iterations <= 0 || framecount <=0 || framecount > 10000 || resolution < 0 || (framecount==1 && (initialscale != finalscale))) {
		printf("The threshold, scale, and max_iterations must be > 0");
		printUsage(argv);
		return 1;
	}

	u_int64_t size = 2 * resolution + 1;
	
	// Allocate memory to store images
	u_int64_t **output;
	output = (u_int64_t **)malloc(framecount * sizeof(u_int64_t *));
	for(int i=0;i<framecount;i++) output[i] = (u_int64_t *)malloc(size * size * sizeof(u_int64_t));
	if (output == NULL) {
		printf("Unable to allocate %lu bytes\n", size * size * sizeof(u_int64_t));
		return 1;
	}







	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

	printf("Running MandelMovie\n");
	MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, output);



	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	
	int res;
	char* num;
	for(int i=0;i < framecount;i++) {
		char outputf[100]="/frame0000";
		char outputfile[100];
		char extension[100]=".ppm";
		if (asprintf(&num, "%d", i) == -1) perror("asprintf");
		strcpy(outputfile, outputfolder);
		strcat(outputfile, outputf);
		strcat(outputfile, num);
		strcat(outputfile, extension); 
		free(num);
		res = writeP6File(colorfile, size, outputfile, output[i]);
	}
	



	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/

	for(int i=0;i<framecount;i++) free(output[i]);
	free(output);
	free(center);




	return 0;
}

