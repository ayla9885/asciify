#include "helpers.h"
#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Image* image_open(char *file_name) {
	Image *img = malloc(sizeof(Image));
	if (img == NULL) {
		return img;
	}
	img->data = stbi_load(file_name, &img->width, &img->height, NULL, 3);
	img->channels = 3;
	return img;
}

void image_free(Image *img) {
	stbi_image_free(img->data);
	free(img);
}

Pixel* image_get_pixel(Image *img, int x, int y) {
	Pixel *pix = malloc(sizeof(Pixel));
	if (pix == NULL) {
		return pix;
	}
	// make sure x and y are inside the image
	x = min(max(x, 0), img->width);
	y = min(max(y, 0), img->height);
	int datalen = img->width*img->height*img->channels;
	int index = (y*img->width*img->channels) + (x*img->channels);
	// make sure index is not out of bounds, this shouldn't happen
	// but just want to be safe
	index = min(max(index, 0), datalen);

	if (img->channels == 3) {
		pix->red = img->data[index];
		pix->green = img->data[index+1];
		pix->blue = img->data[index+2];
	} else if (img->channels == 1) {
		pix->red = img->data[index];
		pix->green = img->data[index];
		pix->blue = img->data[index];
	}
	return pix;
}

int pixel_get_intensity(Pixel *pix) {
	return (pix->red + pix->green + pix->blue) / 3;
}
