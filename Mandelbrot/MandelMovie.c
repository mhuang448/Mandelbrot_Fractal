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
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>
#include <string.h> 
//#include <ColorMapInput.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}

void freeColors(int ** colors, int numColors) {
	for (int i = 0; i < numColors; i++) {
		free(colors[i]);
	}
	free(colors);
}
/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
	//double r = exp(log(finalscale)/(framecount - 1));
	//double r = pow (((long double) finalscale / (long double)initialscale), (1.0 / ((long double) framecount - 1.0)));
	for (int i = 0; i < framecount; i++) {
		//output[i] = malloc(sizeof(u_int64_t) * (pow(2 * resolution + 1, 2));
		//Mandelbrot(threshold, max_iterations, center, (long double) initialscale * powl(((long double) finalscale / (long double)initialscale), ((long double) i/ ((long double) framecount - 1.0))), resolution, output[i]);
		Mandelbrot(threshold, max_iterations, center, initialscale * exp((double) i / (double) (framecount - 1) * (log(finalscale / initialscale))), resolution, output[i]);
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

	//YOUR CODE HERE 
	if (argc != 11) {
                printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
                printUsage(argv);
                return 1;
        }
	double  threshold = atof(argv[1]);
	u_int64_t max_iterations = (u_int64_t)atoi(argv[2]);
	double real = atof(argv[3]);
	double imag= atof(argv[4]);
	double initial_scale = atof(argv[5]);
	double final_scale = atof(argv[6]);
	int frame_count = atoi(argv[7]);;
	u_int64_t resolution = (u_int64_t)atoi(argv[8]);
	char * output_folder = argv[9];
	char * color_file = argv[10];
	if ((threshold <= 0) || (max_iterations <= 0) || (initial_scale <= 0) || (final_scale <= 0)) {
		return 1;
	}
	if ((frame_count > 10000) || (frame_count <= 0)) {
		return 1;
	}
	if (resolution < 0) {
		return 1;
	}
	if ((frame_count == 1) && (initial_scale != final_scale)) {
		return 1;
	}

	ComplexNumber * center = newComplexNumber(real, imag);
        if (center == NULL) {
                printf("Not enough space to create Complex Number center");
		return 1;
        }
	
	//size of each Mandelbrot output array
	u_int64_t size = 2 * resolution + 1;
	//check if colorfile is valid using FileToColorMap method
	int numColors;
	//int  ** colors = (int **) FileToColorMap(color_file, &numColors);
	uint8_t  ** colors = FileToColorMap(color_file, &numColors);
	if (colors == NULL) {
		freeComplexNumber(center);
		return 1;
	}

	//STEP 2: Run MandelMovie on the correct arguments.
        /*
        MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
        If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
        */

        //allocating space for output
	u_int64_t ** output = (u_int64_t **) malloc(sizeof(u_int64_t *) * frame_count);
        if (output == NULL) {
		freeComplexNumber(center);
		freeColors(colors, numColors);
                return 1;
        }

	int i = 0;

	while (i < frame_count) {
		u_int64_t * iteration = (u_int64_t *) malloc(sizeof(u_int64_t) * size * size);
		if (iteration == NULL) {
			for (int j = 0; j < i; j++) {
				free(output[j]);
			}
			freeComplexNumber(center);
			free(output);
			freeColors(colors, numColors);
			return 1;
		}
		output[i] = iteration;
		i++;
		
	}
	
	//running MandelMovie
	MandelMovie(threshold, max_iterations, center, initial_scale, final_scale, frame_count, resolution, output); 
	//fopen("%s/%05d",output_folder,  i




	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/

	//YOUR CODE HERE
	/*
	int * black = (int *) malloc(sizeof(int) * 3);
	//uint8_t * black = (uint8_t *) malloc(sizeof(uint8_t) * 3);
	if (black == NULL) {
        	for (int j = 0; j < frame_count; j++) {
                        free(output[j]);
                }
                free(output);
		freeComplexNumber(center);
		freeColors(colors, numColors);
                printf("not enough memory to allocate color black");
                return 1;
        }
	black[0] = 0;
	black[1] = 0;
	black[2] = 0;
	*/
	
	uint8_t black[] = {0, 0, 0};
	for (i = 0; i < frame_count; i++) {
		//char * fileName = (char*) malloc((16 + strlen(output_folder)) * sizeof(char));
		char fileName[16 + strlen(output_folder)];
		/*
		if (fileName == NULL) {
			for (int j = 0; j < frame_count; j++) {
				free(output[j]);
			}
			free(output);
			free(black);
			freeComplexNumber(center);
			freeColors(colors, numColors);
			printf("not enough memory to allocate fileName");
			return 1;
		}
		*/
		sprintf(fileName, "%s/frame%05d.ppm", output_folder, i );
		FILE *imgl = fopen(fileName, "w");
		if (imgl == NULL) {
			for (int j = 0; j < frame_count; j++) {
				free(output[j]);
			}
			free(output);
			//free(black);
			freeComplexNumber(center);
			freeColors(colors, numColors);
			//free(fileName);
			printf("not enough memory to allocate image file");
			return 1;
		}
		//create p6 header
		fprintf(imgl, "P6 %lu %lu 255\n", size, size);

		u_int64_t * brot  = output[i];
		for (int j = 0; j < (pow(size, 2)); j++) {
			u_int64_t numIters = brot[j];
			if (numIters == 0) {
				//fwrite((unsigned char *) black, sizeof(unsigned char), 3, fileName);
				fwrite(black, sizeof(char), 3, imgl);	
			} else {
				int * color;
				color = colors[(numIters - 1) % numColors];
				//fwrite((unsigned char *) color, sizeof(unsigned char), 3, fileName);
				fwrite(color, sizeof(char), 3, imgl);
			}
		}
		//free(fileName);
		fclose(imgl);
				
	}

	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	//YOUR CODE HERE 
	//free(black);

	//free output and array inside output
	for (i = 0; i < frame_count; i++) {
		free(output[i]);
	}
	free(output);

	//free colors
	for (i = 0; i < numColors; i++) {
		free(colors[i]);
	}
	free(colors);
	freeComplexNumber(center);

	return 0;
}

