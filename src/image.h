#ifndef IMAGE_H
#define IMAGE_H

struct {
	unsigned char *data;
	int width;
	int height;
	int channels;
} typedef Image;

struct {
	int red;
	int blue;
	int green;
} typedef Pixel;

Image* image_open(char *file_name);
void image_free(Image *img);
Pixel* image_get_pixel(Image *img, int x, int y);
int pixel_get_intensity(Pixel *pix);
void pixel_add_color(Pixel *dest, const Pixel *src);


// TODO: finish this VV
// takes rgb values (0-255) and puts the escape code into put_str.
// char* pixel_get_color_escape(Pixel *pix);

#endif
