#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void intensity_filter(char *text, Image *img, int t_width, int t_height, const char *palate) {
	int c_width = img->width / t_width;
	int c_height = img->height / t_height;
	int index = 0;
	int palate_size = strlen(palate);
	// for each character
	for (int cy=0; cy<t_height; cy++) {
		for (int cx=0; cx<t_width; cx++) {
			int intensity = 0;
			// for each pixel that influences the character
			for (int px=1-c_width/2; px<c_width/2; px++) {
				for (int py=1-c_width; py<c_height/2; py++) {
					// px and py are relative coords of the target pixel
					// from the center pixel
					// x and y are the absolute coords of the pixel
					int x = px + cx*c_width;
					int y = py + cy*c_height;
					Pixel *pix = image_get_pixel(img, x, y);
					intensity += pixel_get_intensity(pix);
					free(pix);
				}
			}
			intensity /= c_width*c_height; // average the value
			int palate_index = (double) intensity/255*palate_size;
			text[index] = palate[palate_index];
			index++;
		}
		text[index] = '\n';
		index++;
	}
	text[index-1] = '\0';
	index++;
}

void edge_filter(char *text, Image *img, int t_width, int t_height, const char *palate) {
	printf("edge filter not implemented\n");
}
