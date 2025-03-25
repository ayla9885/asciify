#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char **argv) {
	const char *img_name = argv[1];

	int width, height;
	unsigned char *img_data = stbi_load(img_name, &width, &height, NULL, 3);

	if (img_data == NULL) {
		printf("error\n");
		return 1;
	}
	printf("no error\n");
	printf("x:%i, y:%i\n", width, height);
	return 0;
}
