#ifndef BMP_H
#define BMP_H

#include <stdint.h>

int bmp_export(char *filename, int height, int width, uint8_t *pixels);

#endif // BMP_H
