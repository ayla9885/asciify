#include "helpers.h"
#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Image* image_open(char *file_name) {
	Image *img = malloc(sizeof(*img));
	if (img == NULL) {
		return NULL;
	}
	unsigned char *raw_data = stbi_load(file_name, &img->width, &img->height, NULL, 3);
	img->channels = 3;
	if (raw_data == NULL) {
		return NULL;
	}

	// turn data into an array of pixels
	img->data = malloc(sizeof (*img->data) * img->width * img->height);
	// this needs to be <= !!!!!!!!!!!!
	for (int i=0, j=0; i <= img->width * img->height * img->channels; i += img->channels, j++) {
		Pixel *new = malloc(sizeof (*new));
		new->red = raw_data[i+0];
		new->green = raw_data[i+1];
		new->blue = raw_data[i+2];
		img->data[j] = new;
	}
	stbi_image_free(raw_data);
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
			Pixel *pix = image_get_pixel(img, x, y);
			pix->red = sobel_filter_red(img, x, y);
			pix->green = sobel_filter_green(img, x, y);
			pix->blue = sobel_filter_blue(img, x, y);
		}
	}
}

int sobel_filter_red(Image *img, int x, int y) {
	int x_gradient = 0;
	int y_gradient = 0;

	x_gradient += -1 * image_get_pixel(img, x-1, y-1)->red;
	x_gradient += -2 * image_get_pixel(img, x-1, y  )->red;
	x_gradient += -1 * image_get_pixel(img, x-1, y+1)->red;
	x_gradient +=  1 * image_get_pixel(img, x+1, y-1)->red;
	x_gradient +=  2 * image_get_pixel(img, x+1, y  )->red;
	x_gradient +=  1 * image_get_pixel(img, x+1, y+1)->red;

	y_gradient += -1 * image_get_pixel(img, x-1, y-1)->red;
	y_gradient += -2 * image_get_pixel(img, x  , y-1)->red;
	y_gradient += -1 * image_get_pixel(img, x+1, y-1)->red;
	y_gradient +=  1 * image_get_pixel(img, x-1, y+1)->red;
	y_gradient +=  2 * image_get_pixel(img, x  , y+1)->red;
	y_gradient +=  1 * image_get_pixel(img, x+1, y+1)->red;

	return min(max(sqrt(pow(x_gradient, 2) + pow(y_gradient, 2)), 0), 255);
}
int sobel_filter_green(Image *img, int x, int y) {
	int x_gradient = 0;
	int y_gradient = 0;

	x_gradient += -1 * image_get_pixel(img, x-1, y-1)->green;
	x_gradient += -2 * image_get_pixel(img, x-1, y  )->green;
	x_gradient += -1 * image_get_pixel(img, x-1, y+1)->green;
	x_gradient +=  1 * image_get_pixel(img, x+1, y-1)->green;
	x_gradient +=  2 * image_get_pixel(img, x+1, y  )->green;
	x_gradient +=  1 * image_get_pixel(img, x+1, y+1)->green;

	y_gradient += -1 * image_get_pixel(img, x-1, y-1)->green;
	y_gradient += -2 * image_get_pixel(img, x  , y-1)->green;
	y_gradient += -1 * image_get_pixel(img, x+1, y-1)->green;
	y_gradient +=  1 * image_get_pixel(img, x-1, y+1)->green;
	y_gradient +=  2 * image_get_pixel(img, x  , y+1)->green;
	y_gradient +=  1 * image_get_pixel(img, x+1, y+1)->green;

	return min(max(sqrt(pow(x_gradient, 2) + pow(y_gradient, 2)), 0), 255);
}
int sobel_filter_blue(Image *img, int x, int y) {
	int x_gradient = 0;
	int y_gradient = 0;

	x_gradient += -1 * image_get_pixel(img, x-1, y-1)->blue;
	x_gradient += -2 * image_get_pixel(img, x-1, y  )->blue;
	x_gradient += -1 * image_get_pixel(img, x-1, y+1)->blue;
	x_gradient +=  1 * image_get_pixel(img, x+1, y-1)->blue;
	x_gradient +=  2 * image_get_pixel(img, x+1, y  )->blue;
	x_gradient +=  1 * image_get_pixel(img, x+1, y+1)->blue;

	y_gradient += -1 * image_get_pixel(img, x-1, y-1)->blue;
	y_gradient += -2 * image_get_pixel(img, x  , y-1)->blue;
	y_gradient += -1 * image_get_pixel(img, x+1, y-1)->blue;
	y_gradient +=  1 * image_get_pixel(img, x-1, y+1)->blue;
	y_gradient +=  2 * image_get_pixel(img, x  , y+1)->blue;
	y_gradient +=  1 * image_get_pixel(img, x+1, y+1)->blue;

	return min(max(sqrt(pow(x_gradient, 2) + pow(y_gradient, 2)), 0), 255);
}

int pixel_get_intensity(Pixel *pix) {
	return (pix->red + pix->green + pix->blue) / 3;
}

void pixel_add_color(Pixel *dest, const Pixel *src) {
	dest->red += src->red;
	dest->green += src->green;
	dest->blue += src->blue;
}
