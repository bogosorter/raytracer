#include <stdio.h>
#include <stdint.h>

#include "bmp.h"

#pragma pack(push, 1)

typedef struct {
    uint16_t type;      // Signature, must be 'BM' (0x4D42)
    uint32_t size;      // Total file size in bytes
    uint16_t reserved1; // Reserved, must be 0
    uint16_t reserved2; // Reserved, must be 0
    uint32_t off_bits;  // Byte offset to start of pixel data
} bmp_file_header_t;

typedef struct {
    uint32_t size;          // Size of this header (40 bytes)
    int32_t  width;         // Image width in pixels
    int32_t  height;        // Image height (positive = bottom-up, negative = top-down)
    uint16_t planes;        // Always 1
    uint16_t bit_count;     // Bits per pixel (24 for RGB)
    uint32_t compression;   // Compression type (0 = none)
    uint32_t image_size;    // Image size (can be 0 for uncompressed)
    int32_t  xres;          // Horizontal resolution (pixels per meter)
    int32_t  yres;          // Vertical resolution
    uint32_t clr_used;      // Colors used (0 = all)
    uint32_t clr_important; // Important colors (0 = all)
} bmp_info_header_t;

#pragma pack(pop)

int bmp_export(char *filename, int height, int width, uint8_t *pixels) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Couldn't open output file %s", filename);
        return -1;
    }

    int row_size = (3 * width + 3) & ~3; // Row size must be a multiple of 4 bytes
    int image_size = row_size * height;

    bmp_file_header_t file_header = {
        .type = 0x4D42, // 'BM'
        .size = sizeof(bmp_file_header_t) + sizeof(bmp_info_header_t) + image_size,
        .reserved1 = 0,
        .reserved2 = 0,
        .off_bits = sizeof(bmp_file_header_t) + sizeof(bmp_info_header_t)
    };
    bmp_info_header_t info_header = {
        .size = sizeof(bmp_info_header_t),
        .width = width,
        .height = -height, // Negative for top-down BMP
        .planes = 1,
        .bit_count = 24, // 24 bits per pixel (RGB)
        .compression = 0, // No compression
        .image_size = image_size,
        .xres = 0,
        .yres = 0,
        .clr_used = 0,
        .clr_important = 0
    };

    if (fwrite(&file_header, sizeof(file_header), 1, file) != 1) {
        fclose(file);
        printf("Error writing BMP file header\n");
        return -1;
    }

    if (fwrite(&info_header, sizeof(info_header), 1, file) != 1) {
        fclose(file);
        printf("Error writing BMP info header\n");
        return -1;
    }

    size_t padding_count = row_size - (width * 3);
    uint8_t padding[3] = {0, 0, 0};

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = (i * width + j) * 3;
            if (fwrite(&pixels[index], sizeof(uint8_t), 3, file) != 3) {
                fclose(file);
                printf("Error writing pixel data\n");
                return -1;
            }
        }

        // Padding to ensure row size is a multiple of 4 bytes
        if (fwrite(padding, sizeof(uint8_t), row_size - width * 3, file) != padding_count) {
            fclose(file);
            printf("Error writing padding\n");
            return -1;
        }
    }

    fclose(file);
    return 0;
}
