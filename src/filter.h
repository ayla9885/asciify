#ifndef FILTER_H
#define FILTER_H

#include "image.h"
#include "string.h"

void intensity_filter(String *text, Image *img, int t_width, int t_height, const char *palate, bool color);
void edge_filter(String *text, Image *img, int t_width, int t_height, const char *palate, bool color);

#endif
