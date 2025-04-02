#include "helpers.h"
#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Image* image_open(char *file_name) {
	Image *img = malloc(sizeof(*img));
	if (img == NULL) {
		return img;
	}
	unsigned char *data = stbi_load(file_name, &img->width, &img->height, NULL, 3);
	img->channels = 3;
	// turn data into an array of pixels
	img->data = malloc(sizeof (*img->data) * img->width * img->height);
	// this needs to be <= !!!!!!!!!!!!
	for (int i=0, j=0; i <= img->width * img->height * img->channels; i += img->channels, j++) {
		Pixel *new = malloc(sizeof (*new));
		new->red = data[i+0];
		new->green = data[i+1];
		new->blue = data[i+2];
		img->data[j] = new;
	}
	stbi_image_free(data);
	return img;
}

void image_free(Image *img) {
	for (int i=0; i<img->height*img->width; i++) {
		free(img->data[i]);
	}
	free(img->data);
	free(img);
}

Pixel* image_get_pixel(Image *img, int x, int y) {
	// make sure x and y are inside the image
	x = min(max(x, 0), img->width);
	y = min(max(y, 0), img->height);
	int datalen = img->width*img->height;
	int index = (y*img->width) + x;
	// make sure index is not out of bounds, this shouldn't happen
	// but just want to be safe
	index = min(max(index, 0), datalen);
	return img->data[index];
}

int image_get_intensity(Image *img, int x, int y) {
	Pixel *pix = image_get_pixel(img, x, y);
	int intensity = pixel_get_intensity(pix);
	return intensity;
}

// stores x gradient in red channel, 
// y gradient in green channel, and
// color intensity in blue channel.
void image_edge_detect(Image *img) {
	for (int x=0; x<img->width; x++) {
		for (int y=0; y<img->height; y++) {
		}
	}
}

int pixel_get_intensity(Pixel *pix) {
	return (pix->red + pix->green + pix->blue) / 3;
}

void pixel_add_color(Pixel *dest, const Pixel *src) {
	dest->red += src->red;
	dest->green += src->green;
	dest->blue += src->blue;
}
