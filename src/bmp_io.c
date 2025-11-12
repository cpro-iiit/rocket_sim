/**
 * bmp_io.c - BMP Image File I/O Functions
 */

#include "nbody.h"

/**
 * Writes a 24-bit BMP image file
 * 
 * @param filename Output filename
 * @param img      Pixel array (row-major order)
 * @param w        Image width
 * @param h        Image height
 */
void write_bmp(const char *filename, Pixel *img, int w, int h) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }
    
    // BMP rows must be padded to multiples of 4 bytes
    int row_size = (w * 3 + 3) & ~3;
    int img_size = row_size * h;
    int file_size = 54 + img_size;
    
    // Construct 54-byte BMP header
    unsigned char header[54] = {
        'B','M',
        file_size & 0xFF, (file_size >> 8) & 0xFF, 
        (file_size >> 16) & 0xFF, (file_size >> 24) & 0xFF,
        0,0,0,0,
        54,0,0,0,
        40,0,0,0,
        w & 0xFF, (w >> 8) & 0xFF, (w >> 16) & 0xFF, (w >> 24) & 0xFF,
        h & 0xFF, (h >> 8) & 0xFF, (h >> 16) & 0xFF, (h >> 24) & 0xFF,
        1,0,
        24,0,
        0,0,0,0,
        img_size & 0xFF, (img_size >> 8) & 0xFF, 
        (img_size >> 16) & 0xFF, (img_size >> 24) & 0xFF,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
    };
    
    fwrite(header, 1, 54, f);
    
    // BMP format stores pixels bottom-to-top
    unsigned char padding[3] = {0, 0, 0};
    int pad_size = row_size - w * 3;
    
    for (int y = h - 1; y >= 0; y--) {
        for (int x = 0; x < w; x++) {
            Pixel *p = &img[y * w + x];
            fwrite(p, 3, 1, f);
        }
        if (pad_size > 0) {
            fwrite(padding, 1, pad_size, f);
        }
    }
    
    fclose(f);
}