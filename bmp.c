#include <stdio.h>
#include <stdint.h>

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

int main() {
    FILE *f = fopen("out.bmp", "wb");
    
    int width = 1920, height = 1080;
    int rowSize = (3 * width + 3) & ~3;
    int imageSize = rowSize * height;

    bmp_file_header_t fileHeader = {
        .type = 0x4D42,
        .size = sizeof(bmp_file_header_t) + sizeof(bmp_info_header_t) + imageSize,
        .off_bits = sizeof(bmp_file_header_t) + sizeof(bmp_info_header_t)
    };

    bmp_info_header_t infoHeader = {
        .size = sizeof(bmp_info_header_t),
        .width = width,
        .height = -height,  // top-down
        .planes = 1,
        .bit_count = 24,
        .compression = 0,
        .image_size = imageSize
    };

    fwrite(&fileHeader, sizeof(fileHeader), 1, f);
    fwrite(&infoHeader, sizeof(infoHeader), 1, f);

    uint8_t pixels[height * width * 3];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = (i * width + j) * 3;
            pixels[index] = 0xff;
            pixels[index + 1] = 0xff;
            pixels[index + 2] = 0xff;
        }
    }

    uint8_t padding[3] = {};
    for (int y = 0; y < height; ++y) {
        fwrite(&pixels[y * width * 3], 3, width, f);
        fwrite(padding, 1, rowSize - width * 3, f);
    }

    fclose(f);
    return 0;
}
