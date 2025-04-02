#ifndef IMAGE_H
#define IMAGE_H

struct {
	int red;
	int blue;
	int green;
} typedef Pixel;

struct {
	Pixel **data;
	int width;
	int height;
	int channels;
} typedef Image;

Image* image_open(char *file_name);
void image_free(Image *img);
Pixel* image_get_pixel(const Image *img, int x, int y);
int image_get_intensity(const Image *img, int x, int y);
void image_edge_detect(Image *img);

int pixel_get_intensity(const Pixel *pix);
void pixel_add_color(Pixel *dest, const Pixel *src);

int sobel_x_kernel(const Image *img, int x, int y);
int sobel_y_kernel(const Image *img, int x, int y);

#endif
