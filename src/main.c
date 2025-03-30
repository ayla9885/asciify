#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "image.h"
#include "filter.h"
#include "helpers.h"

enum Mode {
	EDGE,
	INTENSITY,
};

const int DEFAULT_WIDTH = 50;
//const int DEFAULT_HEIGHT = 30;
const char *OPTS = "cs:m:f:";
const char PALATE[] = " .,-=oab0#@";
int TRUECOLOR = false;

int parse_args(int argc, char **argv, char **put_name, double *put_scale, int *put_width, int *put_height, char **put_outfile, enum Mode *put_mode, int *channel_num);

int main(int argc, char **argv) {
	char *img_name; 
	char *out_name;
	int channel_num;
	enum Mode mode;
	double target_scale;
	int target_width;
	int target_height;
	int err = parse_args(argc, argv, &img_name, &target_scale, &target_height, &target_width, &out_name, &mode, &channel_num);
	if (err) {
		return 1;
	}

	Image *img = image_open(img_name);
	if (img == NULL || img->data == NULL) {
		printf("%s: Error opening image '%s'\n", argv[0], img_name);
		return 2;
	}
	
	// determine dimensions of output
	if (target_scale > 0) {
		target_width = img->width * target_scale;
		target_height = img->height * target_scale / 2; // divide by two because terminal cells are taller than they are wide
	} else {
		if (target_width == 0) {
			if (target_height != 0) {
				target_width = img->width/img->height * target_height;
			} else {
				target_width = DEFAULT_WIDTH;
			}
		}
		if (target_height == 0) {
			if (target_width != 0) { // target_width is always gonna be above 0 because either the user provided one, or it was set above
				target_height = (double) img->height/img->width * target_width / 2; // divide by two because terminal cells are taller than they are wide
			} 
		}
	}

	char *text = calloc(sizeof(char), (target_height*target_width) + (target_height));
	if (text == NULL) {
		return 3;
	}
	switch (mode) {
		case EDGE:
			edge_filter(text, img, target_width, target_height, PALATE);
			break;
		case INTENSITY:
			intensity_filter(text, img, target_width, target_height, PALATE);
			break;
	}
	printf("%s", text);

	image_free(img);
	return 0;
}

//todo: finish
int parse_args(int argc, char **argv, char **put_name, double *put_scale, int *put_width, int *put_height, char **put_outfile, enum Mode *put_mode, int *channel_num) {
	// set default values
	*put_outfile = NULL;
	*put_scale = 0;
	*put_width = 0;
	*put_height = 0;
	*channel_num = 1;

	while (optind < argc) {
		int opt = getopt(argc, argv, OPTS);
		if (opt != -1) { // if next arg is an option
			switch (opt) {
				case 'c':	// colorize
					*channel_num = 3;
					break;
				case 's':	// scale
					*put_scale = atof(optarg);
					break;
				case 'm':	// mode
					if (strcmp_nocase(optarg, "edge") == 0) {
						*put_mode = EDGE;
					} else if (strcmp_nocase(optarg, "intensity") == 0) {
						*put_mode = INTENSITY;
					} else {
						printf("%s: Error, unrecognized mode -- %s\n", argv[0], optarg); // todo: make actual error message
						return 1;
					}
					break;
				case 'f': // output file 
						*put_outfile = optarg;
					break;
				default:
					return 1;
			}
		} else {
			*put_name = argv[optind];
			optind++;
		}
	}
	// check for truecolor support
	char *color_mode = getenv("COLORTERM");
	if (color_mode != NULL) {
		TRUECOLOR = strcmp(color_mode, "truecolor") == 0 || strcmp(color_mode, "24bit") == 0;
	}
	return 0;
}

// void intensity_filter(char *text, unsigned char *data, int s_width, int s_height, int t_width, int t_height, int channel_num) {
// 	int c_width = s_width / t_width;
// 	int c_height = s_height / t_height;
// 	int data_len = s_width*s_height*channel_num;
// 	// for each character in the final output
// 	for (int i=0; i<t_height; i++) {
// 		for (int j=0; j<t_width; j++) {
// 			char c = '0';
// 			int intensity = 0;
// 			int r_intensity = 0;
// 			int g_intensity = 0;
// 			int b_intensity = 0;
// 			// for each pixel that influence the current character
// 			for (int x=1-c_width/2; x<c_width/2; x++) {
// 				for (int y=1-c_height/2; y<c_height/2; y++) {
// 					// the two inner min maxes make sure the x and y coords dont
// 					// go outside the image. the outer min max is another safety
// 					// check to make sure the index doesn't go outisde the
// 					// bounds of the data
// 					int data_index = min(max( min(max(i*c_height+y, 0), s_height)*s_width*channel_num + min(max(j*c_width+x, 0), s_width)*channel_num, 0), data_len);
// 					if (channel_num == 1) {
// 						intensity += data[data_index];
// 					} else {
// 						r_intensity += data[data_index];
// 						g_intensity += data[data_index+1];
// 						b_intensity += data[data_index+2];
// 					}
// 				}
// 			}
// 			int pallate_index;
// 			if (channel_num == 1) {
// 				intensity /= c_width*c_height; // turn into average
// 				pallate_index = (double) intensity/255*PALLETE_SIZE;
// 			} else {
// 				r_intensity /= c_width*c_height; // turn into average
// 				g_intensity /= c_width*c_height; // turn into average
// 				b_intensity /= c_width*c_height; // turn into average
// 				pallate_index = (double) r_intensity/255*PALLETE_SIZE;
// 			}
// 			c = PALLETE[pallate_index];
// 			text[i*(t_width+1) + j] = c;
// 		}
// 		text[i*(t_width+1)+t_width] = '\n';
// 	}
// 	text[t_height*(t_width+1)-1] = '\0';
// }



