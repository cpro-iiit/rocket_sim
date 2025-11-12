/**
 * Exercise 3.2 - Reload and Plot Trails
 * 
 * This program reads saved trajectory data and renders it to a BMP image
 * without re-running the simulation.
 * 
 * Usage: gcc -o plot_trails plot_trails.c -lm
 *        ./plot_trails rocket_trails.bin output.bmp
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 800

typedef struct {
    unsigned char b, g, r;
} Pixel;

/**
 * Write BMP file
 */
void write_bmp(const char *filename, Pixel *img, int w, int h) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }
    
    int row_size = (w * 3 + 3) & ~3;
    int img_size = row_size * h;
    int file_size = 54 + img_size;
    
    unsigned char header[54] = {
        'B','M',
        file_size & 0xFF, (file_size >> 8) & 0xFF, (file_size >> 16) & 0xFF, (file_size >> 24) & 0xFF,
        0,0,0,0,
        54,0,0,0,
        40,0,0,0,
        w & 0xFF, (w >> 8) & 0xFF, (w >> 16) & 0xFF, (w >> 24) & 0xFF,
        h & 0xFF, (h >> 8) & 0xFF, (h >> 16) & 0xFF, (h >> 24) & 0xFF,
        1,0,
        24,0,
        0,0,0,0,
        img_size & 0xFF, (img_size >> 8) & 0xFF, (img_size >> 16) & 0xFF, (img_size >> 24) & 0xFF,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0
    };
    
    fwrite(header, 1, 54, f);
    
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

/**
 * Draw a line using Bresenham's algorithm
 */
void draw_line(Pixel *img, int x1, int y1, int x2, int y2, 
               unsigned char r, unsigned char g, unsigned char b) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int x = x1, y = y1;
    while (1) {
        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
            img[y * WIDTH + x].r = r;
            img[y * WIDTH + x].g = g;
            img[y * WIDTH + x].b = b;
        }
        
        if (x == x2 && y == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

/**
 * Draw a filled circle
 */
void draw_circle(Pixel *img, int cx, int cy, int radius,
                unsigned char r, unsigned char g, unsigned char b) {
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                int x = cx + dx;
                int y = cy + dy;
                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                    img[y * WIDTH + x].r = r;
                    img[y * WIDTH + x].g = g;
                    img[y * WIDTH + x].b = b;
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    const char *input_file = "rocket_trails.bin";
    const char *output_file = "plotted_trails.bmp";
    
    if (argc > 1) input_file = argv[1];
    if (argc > 2) output_file = argv[2];
    
    printf("====================================\n");
    printf("Trajectory Plotting Tool\n");
    printf("====================================\n");
    printf("Input: %s\n", input_file);
    printf("Output: %s\n\n", output_file);
    
    // Open binary file
    FILE *f = fopen(input_file, "rb");
    if (!f) {
        printf("Error: Could not open %s\n", input_file);
        return 1;
    }
    
    // Read number of rockets
    int n_rockets;
    if (fread(&n_rockets, sizeof(int), 1, f) != 1) {
        printf("Error reading file\n");
        fclose(f);
        return 1;
    }
    
    printf("Loading %d rocket trajectories...\n", n_rockets);
    
    // Allocate image
    Pixel *img = (Pixel *)calloc(WIDTH * HEIGHT, sizeof(Pixel));
    if (!img) {
        printf("Memory allocation failed\n");
        fclose(f);
        return 1;
    }
    
    // Draw grid
    for (int i = 0; i < WIDTH; i += 50) {
        for (int j = 0; j < HEIGHT; j++) {
            img[j * WIDTH + i].r = 30;
            img[j * WIDTH + i].g = 30;
            img[j * WIDTH + i].b = 30;
        }
    }
    for (int j = 0; j < HEIGHT; j += 50) {
        for (int i = 0; i < WIDTH; i++) {
            img[j * WIDTH + i].r = 30;
            img[j * WIDTH + i].g = 30;
            img[j * WIDTH + i].b = 30;
        }
    }
    
    // Draw center marker
    draw_circle(img, WIDTH/2, HEIGHT/2, 5, 255, 255, 100);
    
    double scale = 50.0;
    
    // Colors for different rockets
    unsigned char colors[][3] = {
        {255, 100, 100},  // Red
        {100, 255, 100},  // Green
        {100, 100, 255},  // Blue
        {255, 255, 100},  // Yellow
        {255, 100, 255},  // Magenta
        {100, 255, 255},  // Cyan
        {255, 150, 100},  // Orange
        {150, 100, 255},  // Purple
        {100, 255, 150},  // Light green
        {255, 100, 150}   // Pink
    };
    
    // Load and plot each rocket
    for (int i = 0; i < n_rockets; i++) {
        int trail_length;
        if (fread(&trail_length, sizeof(int), 1, f) != 1) {
            printf("Error reading rocket %d\n", i);
            break;
        }
        
        double *trail_x = (double *)malloc(trail_length * sizeof(double));
        double *trail_y = (double *)malloc(trail_length * sizeof(double));
        
        if (!trail_x || !trail_y) {
            printf("Memory allocation failed\n");
            break;
        }
        
        if (fread(trail_x, sizeof(double), trail_length, f) != trail_length ||
            fread(trail_y, sizeof(double), trail_length, f) != trail_length) {
            printf("Error reading trajectory %d\n", i);
            free(trail_x);
            free(trail_y);
            break;
        }
        
        printf("  Rocket %d: %d points\n", i, trail_length);
        
        // Choose color
        unsigned char r = colors[i % 10][0];
        unsigned char g = colors[i % 10][1];
        unsigned char b = colors[i % 10][2];
        
        // Draw trajectory
        for (int j = 0; j < trail_length - 1; j++) {
            int px1 = (int)(trail_x[j] * scale + WIDTH / 2);
            int py1 = (int)(trail_y[j] * scale + HEIGHT / 2);
            int px2 = (int)(trail_x[j + 1] * scale + WIDTH / 2);
            int py2 = (int)(trail_y[j + 1] * scale + HEIGHT / 2);
            
            // Gradient effect
            int brightness = 100 + (155 * j) / trail_length;
            unsigned char br = (r * brightness) / 255;
            unsigned char bg = (g * brightness) / 255;
            unsigned char bb = (b * brightness) / 255;
            
            draw_line(img, px1, py1, px2, py2, br, bg, bb);
        }
        
        // Mark start position
        int start_x = (int)(trail_x[0] * scale + WIDTH / 2);
        int start_y = (int)(trail_y[0] * scale + HEIGHT / 2);
        draw_circle(img, start_x, start_y, 4, 100, 255, 100);
        
        // Mark end position
        int end_x = (int)(trail_x[trail_length-1] * scale + WIDTH / 2);
        int end_y = (int)(trail_y[trail_length-1] * scale + HEIGHT / 2);
        draw_circle(img, end_x, end_y, 6, r, g, b);
        
        free(trail_x);
        free(trail_y);
    }
    
    fclose(f);
    
    // Write output image
    write_bmp(output_file, img, WIDTH, HEIGHT);
    free(img);
    
    printf("\n====================================\n");
    printf("Plotted trajectories saved to:\n");
    printf("  %s\n", output_file);
    printf("====================================\n");
    
    return 0;
}