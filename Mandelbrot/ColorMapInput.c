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
	FILE * ptr = fopen(colorfile, "r");
        if (ptr == NULL) {
                return NULL;
        }
        int size;
        int byte = fscanf(ptr, "%d", &size);
        if (byte <= 0) {
                return NULL;
        }
        uint8_t ** ret = malloc(sizeof(uint8_t *) * size);
        *colorcount = size;
        int count = 0;
        while (count < size) {
                int red;
                int green;
                int blue;
                uint8_t uRed;
                uint8_t uGreen;
                uint8_t uBlue;

                uint8_t * color = malloc(sizeof(uint8_t) * 3);

                //scan red
                byte = fscanf(ptr, "%d", &red);
                if (byte <= 0) {
			for (int j = 0; j < count; j++) {
				free(ret[j]);
			}
			free(color);
			free(ret);
                        return NULL;
                }
                uRed = red;
                //scan green
                byte = fscanf(ptr, "%d", &green);
                if (byte <= 0) {
			for (int j = 0; j < count; j++) {
                                free(ret[j]);
                        }
                        free(color);
                        free(ret);
                        return NULL;
                }
                uGreen = green;
                //scan blue
                byte = fscanf(ptr, "%d", &blue);
                if (byte <= 0) {
			for (int j = 0; j < count; j++) {
                                free(ret[j]);
                        }
                        free(color);
                        free(ret);
                        return NULL;
                } 
                uBlue = blue;

                color[0] = uRed;
                color[1] = uGreen;
                color[2] = uBlue;

                ret[count] = color;

                count ++;

        }
        fclose(ptr);
        return ret;
	
}


