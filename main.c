#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum Mode {
	EDGE,
	INTENSITY,
};

const int DEFUALT_WIDTH = 30;
const int DEFAULT_HEIGHT = 30;
const char *OPTS = "cs:m:f:";
const char PALLETE[] = " ░▒▓█";
const int PALLETE_SIZE=5;
int TRUECOLOR = false;

int parse_args(int argc, char **argv, char **put_name, double *put_scale, char **put_outfile, enum Mode *put_mode, int *channel_num);
void edge_filter(char *text, unsigned char *data, int s_width, int s_height, int t_width, int t_height, int channel_num);
void intensity_filter(char *text, unsigned char *data, int s_width, int s_height, int t_width, int t_height, int channel_num);
int strcmp_nocase(const char *a, const char *b);
int min(int a, int b);
int max(int a, int b); 

int main(int argc, char **argv) {
	printf("\x1b[38;2;40;177;249mTRUECOLOR\x1b[0m\n");
	char *img_name; 
	char *out_name;
	int channel_num;
	enum Mode mode;
	double target_scale;
	int err = parse_args(argc, argv, &img_name, &target_scale, &out_name, &mode, &channel_num);
	if (err) {
		printf("arg error\n");
		return 1;
	}

	int source_width, source_height;
	unsigned char *img_data = stbi_load(img_name, &source_width, &source_height, NULL, channel_num);
	if (img_data == NULL) {
		printf("error\n");
		return 2;
	}
	printf("no error\n");
	printf("x:%i, y:%i\n", source_width, source_height);
	
	int target_width = source_width * target_scale;
	int target_height = source_height * target_scale / 2; // divide by two because terminal cells are taller than they are wide

	char *text = calloc(sizeof(char), (target_height*target_width) + (target_height));
	if (text == NULL) {
		return 3;
	}
	switch (mode) {
		case EDGE:
			edge_filter(text, img_data, source_width, source_height, target_width, target_height, channel_num);
			break;
		case INTENSITY:
			intensity_filter(text, img_data, source_width, source_height, target_width, target_height, channel_num);
			break;
	}
	printf("%s", text);

	return 0;
}

//todo: finish
int parse_args(int argc, char **argv, char **put_name, double *put_scale, char **put_outfile, enum Mode *put_mode, int *channel_num) {
	// set default values
	*put_outfile = NULL;
	*put_scale = 1;
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
						printf("unrecognized mode\n"); // todo: make actual error message
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
	printf("%i\n", TRUECOLOR);
	return 0;
}

void edge_filter(char *text, unsigned char *data, int s_width, int s_height, int t_width, int t_height, int channel_num) {
	for (;;) {

	}
}

void intensity_filter(char *text, unsigned char *data, int s_width, int s_height, int t_width, int t_height, int channel_num) {
	int c_width = s_width / t_width;
	int c_height = s_height / t_height;
	int data_len = s_width*s_height*channel_num;
	// for each character in the final output
	for (int i=0; i<t_height; i++) {
		for (int j=0; j<t_width; j++) {
			char c = '0';
			int intensity = 0;
			int r_intensity = 0;
			int g_intensity = 0;
			int b_intensity = 0;
			// for each pixel that influence the current character
			for (int x=1-c_width/2; x<c_width/2; x++) {
				for (int y=1-c_height/2; y<c_height/2; y++) {
					// the two inner min maxes make sure the x and y coords dont
					// go outside the image. the outer min max is another safety
					// check to make sure the index doesn't go outisde the
					// bounds of the data
					int data_index = min(max( min(max(i*c_height+y, 0), s_height)*s_width*channel_num + min(max(j*c_width+x, 0), s_width)*channel_num, 0), data_len);
					if (channel_num == 1) {
						intensity += data[data_index];
					} else {
						r_intensity += data[data_index];
						g_intensity += data[data_index+1];
						b_intensity += data[data_index+2];
					}
				}
			}
			int pallate_index;
			if (channel_num == 1) {
				intensity /= c_width*c_height; // turn into average
				pallate_index = (double) intensity/255*PALLETE_SIZE;
			} else {
				r_intensity /= c_width*c_height; // turn into average
				g_intensity /= c_width*c_height; // turn into average
				b_intensity /= c_width*c_height; // turn into average
				pallate_index = (double) r_intensity/255*PALLETE_SIZE;
			}
			c = PALLETE[pallate_index];
			text[i*(t_width+1) + j] = c;
		}
		text[i*(t_width+1)+t_width] = '\n';
	}
	text[t_height*(t_width+1)-1] = '\0';
}

// Compares string a and string b, ignoring case
int strcmp_nocase(const char *a, const char *b) {
	int alen = strlen(a);
	int blen = strlen(b);
	int dif = 0;
	for (int i=0; i<alen && i<blen; i++) {
		dif = tolower(a[i]) - tolower(b[i]);
		if (dif != 0) {
			return dif;
		}
	}
	if (alen != blen) {
		return alen - blen;
	}
	return dif;
}

// TODO: finish this VV
// takes rgb values (0-255) and puts the escape code into put_str.
// char* get_color_escape(int r, int b, int g) {
// 	r = min(max(r, 0), 255);
// }

int min(int a, int b) {
	if (a<b) {
		return a;
	} else {
		return b;
	}
}

int max(int a, int b) {
	if (a>b) {
		return a;
	} else {
		return b;
	}
}
