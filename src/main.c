#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stb/stb_image.h>

#include "string.h"
#include "image.h"
#include "filter.h"
#include "helpers.h"

enum Mode {
	EDGE,
	INTENSITY,
};

enum ColorMode {
	TRUECOLOR,
};

struct Settings {
	char *img_name;
	char *out_name;
	bool colorize;
	enum ColorMode color_support;
	enum Mode mode;
	double target_scale;
	int target_width;
	int target_height;
};

const int DEFAULT_WIDTH = 50;
//const int DEFAULT_HEIGHT = 30;
const char *OPTS = "hcs:m:f:d:";
const char PALATE[] = " .,-=oab0#@";

int parse_args(int argc, char **argv, struct Settings *settings);
int get_dimensions(char *arg, int *width, int* height);

int main(int argc, char **argv) {
	struct Settings settings;
	int err = parse_args(argc, argv, &settings);
	if (err) {
		return 1;
	}

	Image *img = image_open(settings.img_name);
	if (img == NULL || img->data == NULL) {
		printf("%s: Error opening image '%s'\n", argv[0], settings.img_name);
		printf("%s\n", stbi_failure_reason());
		return 2;
	}
	
	// determine dimensions of output
	if (settings.target_scale > 0) {
		settings.target_width = img->width * settings.target_scale;
		settings.target_height = img->height * settings.target_scale / 2; // divide by two because terminal cells are taller than they are wide
	} else {
		if (settings.target_width == 0) {
			if (settings.target_height != 0) {
				settings.target_width = img->width/img->height * settings.target_height;
			} else {
				settings.target_width = DEFAULT_WIDTH;
			}
		}
		if (settings.target_height == 0) {
			if (settings.target_width != 0) { // target_width is always gonna be above 0 because either the user provided one, or it was set above
				settings.target_height = (double) img->height/img->width * settings.target_width / 2; // divide by two because terminal cells are taller than they are wide
			} 
		}
	}

	// open output file if needed
	FILE *out_file;
	if (settings.out_name) {
		out_file = fopen(settings.out_name, "w");
		if (!out_file) {
			return 5;
		}
	}

	String *text = string_new();
	switch (settings.mode) {
		case EDGE:
			edge_filter(text, img, settings.target_width, settings.target_height, PALATE, settings.colorize);
			break;
		case INTENSITY:
			intensity_filter(text, img, settings.target_width, settings.target_height, PALATE, settings.colorize);
			break;
	}
	if (settings.out_name != NULL) {
		string_println(text);
	} else {
		string_fwrite(text, out_file);
	}

	image_free(img);
	string_free(text);
	return 0;
}

//todo: finish
int parse_args(int argc, char **argv, struct Settings *settings) {
	// set default values
	settings->mode = INTENSITY;
	settings->img_name = NULL;
	settings->out_name = NULL;
	settings->target_scale = 0;
	settings->target_width = 0;
	settings->target_height = 0;
	settings->colorize = false;

	while (optind < argc) {
		int opt = getopt(argc, argv, OPTS);
		if (opt != -1) { // if next arg is an option
			switch (opt) {
				case 'c':	// colorize
					settings->colorize = 3;
					break;
				case 's':	// scale
					settings->target_scale = atof(optarg);
					printf("scale: %f\n", settings->target_scale);
					break;
				case 'm':	// mode
					if (strcmp_nocase(optarg, "edge") == 0) {
						settings->mode = EDGE;
					} else if (strcmp_nocase(optarg, "intensity") == 0) {
						settings->mode = INTENSITY;
					} else {
						printf("%s: Error, unrecognized mode -- %s\n", argv[0], optarg); // todo: make actual error message
						return 1;
					}
					break;
				case 'f': // output file 
					settings->out_name = optarg;
					break;
				case 'd': // dimensions. formatted like WIDTHxHEIGHT
					int err = get_dimensions(optarg, &settings->target_width, &settings->target_height);
					if (err) {
						printf("%s: Error, incorrect formatting -- %s\n", argv[0], optarg);
						printf("Usage: -d WIDTHxHEIGHT\nWidth and height must be non-zero");
						return 1;
					}
					break;
				case 'h': // help
					break;
				default:
					return 1;
			}
		} else {
			settings->img_name = argv[optind];
			optind++;
		}
	}
	// check for truecolor support
	char *color_mode = getenv("COLORTERM");
	if (color_mode != NULL) {
		settings->color_support = strcmp(color_mode, "truecolor") == 0 || strcmp(color_mode, "24bit") == 0;
	}
	return 0;
}

int get_dimensions(char *arg, int *width, int* height) {
	int i = 0;
	int len = strlen(arg);
	// skip preceding white space
	while (i < len && isblank(arg[i])) {
		i++;
	}
	*width = atoi(arg);
	// skip past width
	while (i < len && tolower(arg[i]) != 'x') {
		i++;
	}
	i++;
	// skip whitespace
	while (i < len && isblank(arg[i])) {
		i++;
	}
	*height = atoi(arg + i);
	if (*width == 0 || *height == 0) {
		return 1;
	}
	return 0;
}

