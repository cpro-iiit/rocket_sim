/**
 * render.c - Rendering Functions
 * Converts simulation state to visual representation
 */

#include "nbody.h"

/**
 * Draw a line using Bresenham's line algorithm
 * Used for rendering rocket trajectories
 */
static void draw_line(Pixel *img, int x1, int y1, int x2, int y2,
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
 * Used for rendering celestial bodies and rockets
 */
static void draw_circle(Pixel *img, int cx, int cy, int radius,
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

/**
 * Render the current simulation state to an image
 * 
 * Rendering order:
 * 1. Background grid
 * 2. Rocket trajectories (complete history)
 * 3. Celestial bodies
 * 4. Current rocket positions
 */
void render(Body *bodies, int n_bodies, Rocket *rockets, int n_rockets, 
            Pixel *img, double scale) {
    // Clear image to black
    memset(img, 0, WIDTH * HEIGHT * sizeof(Pixel));
    
    // Draw reference grid
    for (int i = 0; i < WIDTH; i += 50) {
        for (int j = 0; j < HEIGHT; j++) {
            img[j * WIDTH + i].r = 20;
            img[j * WIDTH + i].g = 20;
            img[j * WIDTH + i].b = 20;
        }
    }
    for (int j = 0; j < HEIGHT; j += 50) {
        for (int i = 0; i < WIDTH; i++) {
            img[j * WIDTH + i].r = 20;
            img[j * WIDTH + i].g = 20;
            img[j * WIDTH + i].b = 20;
        }
    }
    
    // Draw complete rocket trajectories
    for (int i = 0; i < n_rockets; i++) {
        // Draw trail as connected line segments
        for (int t = 0; t < rockets[i].trail_length - 1; t++) {
            int px1 = (int)(rockets[i].trail_x[t] * scale + WIDTH / 2);
            int py1 = (int)(rockets[i].trail_y[t] * scale + HEIGHT / 2);
            int px2 = (int)(rockets[i].trail_x[t + 1] * scale + WIDTH / 2);
            int py2 = (int)(rockets[i].trail_y[t + 1] * scale + HEIGHT / 2);
            
            // Color gradient: older parts dimmer, newer parts brighter
            int brightness = 100 + (155 * t) / 
                (rockets[i].trail_length > 0 ? rockets[i].trail_length : 1);
            
            draw_line(img, px1, py1, px2, py2, 
                     brightness, brightness / 2, brightness / 2);
        }
        
        // Draw current rocket position
        if (rockets[i].trail_length > 0) {
            int px = (int)(rockets[i].x * scale + WIDTH / 2);
            int py = (int)(rockets[i].y * scale + HEIGHT / 2);
            
            if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
                // Draw rocket as bright red circle
                draw_circle(img, px, py, 4, 255, 50, 50);
            }
        }
    }
    
    // Draw celestial bodies (planets and stars)
    for (int i = 0; i < n_bodies; i++) {
        int px = (int)(bodies[i].x * scale + WIDTH / 2);
        int py = (int)(bodies[i].y * scale + HEIGHT / 2);
        
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
            // Central body is larger
            int radius = (i == 0) ? 8 : 4;
            
            if (i == 0) {
                // Central star: yellow
                draw_circle(img, px, py, radius, 255, 255, 100);
            } else {
                // Orbiting bodies: blue shades
                unsigned char r = 100 + i * 30;
                draw_circle(img, px, py, radius, r, 150, 255);
            }
        }
    }
}