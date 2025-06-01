#include <stdio.h>
#include <stdint.h>
#include "bmp.h"

int main() {
    int height = 1080;
    int width = 1920;

    uint8_t pixels[height * width * 3];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = (i * width + j) * 3;
            
            pixels[index] = (i * 255) / height; // blue
            pixels[index + 1] = ((i + j) * 255) / (height + width); // green
            pixels[index + 2] = 0; // red
        }
    }

    if (bmp_export("output.bmp", height, width, pixels) != 0) {
        printf("Error exporting BMP file\n");
    }
}
