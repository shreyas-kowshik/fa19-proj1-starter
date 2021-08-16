/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in 
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	/*
	Format :
	P3 width height 255(max_intensity_val)
	pixel_intensity ... * width
	.
	. * height
	.
	pixel_intensity ... * width	
	*/
	int* colorcount;
	int val=0;
	colorcount=&val;
	uint8_t** colors = FileToColorMap(colorfile, colorcount);
	int height = heightpercolor * (*colorcount);
	FILE* fp=fopen(outputfile, "w");
	fprintf(fp, "P3 %d %d 255\n", width, height);
	
	int idx=0;
	for(int h=0;h<height;h++) {
		idx=(h/heightpercolor);
		for(int w=0;w<width-1;w++) {
			fprintf(fp, "%d %d %d ", colors[idx][0], colors[idx][1], colors[idx][2]);
		}
		fprintf(fp, "%d %d %d\n", colors[idx][0], colors[idx][1], colors[idx][2]);
	}

	int res=fclose(fp);
	for(int i = 0;i < (*colorcount);i++) free(colors[i]);
	free(colors);

	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	int* colorcount;
	int val=0;
	colorcount=&val;
	uint8_t** colors = FileToColorMap(colorfile, colorcount);
	int height = heightpercolor * (*colorcount);
	FILE* fp=fopen(outputfile, "w");
	char str1[] = "P6 ";
	char space[] = " ";
	int intensity=255;
	fprintf(fp, "P6 %d %d 255\n", width, height);
	

	int idx=0;
	for(int h=0;h<height;h++) {
		idx=(h/heightpercolor);
		for(int w=0;w<width-1;w++) {
			fwrite(&(colors[idx][0]), sizeof(colors[idx][0]), 1, fp);
			fwrite(&(colors[idx][1]), sizeof(colors[idx][1]), 1, fp);
			fwrite(&(colors[idx][2]), sizeof(colors[idx][2]), 1, fp);
			// fprintf(fp, "%d %d %d ", colors[idx][0], colors[idx][1], colors[idx][2]);
		}
		fwrite(&(colors[idx][0]), sizeof(colors[idx][0]), 1, fp);
		fwrite(&(colors[idx][1]), sizeof(colors[idx][1]), 1, fp);
		fwrite(&(colors[idx][2]), sizeof(colors[idx][2]), 1, fp);
	}

	int res=fclose(fp);
	for(int i = 0;i < (*colorcount);i++) free(colors[i]);
	free(colors);
	

	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)+1];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}




