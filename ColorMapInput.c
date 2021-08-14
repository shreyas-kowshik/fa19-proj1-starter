/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	uint8_t** colors;

	FILE* fp = fopen(colorfile, "r");
	if(fp==NULL) {
              return NULL;
        }

        char *buf = (char *)malloc(100 * sizeof(char));
	uint8_t color;
	int line=-1; // stores line number
	u_int64_t num_colors=0; // keep track
        while(fscanf(fp,"%s", buf)==1) {
		if(line == -1) {
			*colorcount = atoi(buf);
			colors = (uint8_t **)malloc((*colorcount) * sizeof(uint8_t *));
			for(int i = 0;i < (*colorcount);i++)
				colors[i] = (uint8_t *)malloc(3 * sizeof(uint8_t));

		}
		else {
                	color = atoi(buf);
			colors[line/3][line%3] = color;
			num_colors++;
		}
		line++;
        }
	int res = fclose(fp);
	free(buf);

	if((3 * (*colorcount)) != num_colors) {
		for(int i = 0;i < (*colorcount);i++) free(colors[i]);
		free(colors);
		return NULL;
	}

	
	return colors;
	
}


