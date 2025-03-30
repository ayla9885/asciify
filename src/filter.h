#ifndef FILTER_H
#define FILTER_H

#include "image.h"

void intensity_filter(char *text, Image *img, int t_width, int t_height, const char *palate);
void edge_filter(char *text, Image *img, int t_width, int t_height, const char *palate);

#endif
