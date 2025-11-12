/**
 * nbody.h - N-Body Simulation Header File
 * 
 * Location: include/nbody.h
 * Contains all data structures, constants, and function declarations
 */

#ifndef NBODY_H
#define NBODY_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* ============================================================================
 * SIMULATION PARAMETERS
 * ============================================================================ */
#define WIDTH 800              // Image width in pixels
#define HEIGHT 800             // Image height in pixels
#define MAX_BODIES 20          // Maximum number of gravitating bodies
#define MAX_ROCKETS 10         // Maximum number of rockets
#define DT 0.01                // Default time step
#define STEPS 5000             // Default total steps
#define FRAMES 100             // Default number of frames
#define G 1.0                  // Default gravitational constant
#define SOFTENING 0.1          // Softening parameter

/* ============================================================================
 * DATA STRUCTURES
 * ============================================================================ */

/**
 * Body structure representing a gravitating mass
 */
typedef struct {
    double x, y;      // Position in 2D space
    double vx, vy;    // Velocity components
    double ax, ay;    // Acceleration components
    double mass;      // Mass of the body
} Body;

/**
 * Rocket structure representing a test particle
 */
typedef struct {
    double x, y;      // Position in 2D space
    double vx, vy;    // Velocity components
    double ax, ay;    // Acceleration components
    int active;       // 1 if rocket is active, 0 if inactive
    double *trail_x;  // Array to store X positions
    double *trail_y;  // Array to store Y positions
    int trail_length; // Current length of trail
    int trail_capacity; // Maximum capacity of trail arrays
} Rocket;

/**
 * Pixel structure for BMP image format
 */
typedef struct {
    unsigned char b, g, r;  // Blue, Green, Red color channels
} Pixel;

/**
 * Simulation configuration structure
 */
typedef struct {
    double dt;
    int steps;
    int frames;
    int save_interval;
    double g;
} SimConfig;

/* ============================================================================
 * FUNCTION DECLARATIONS - bmp_io.c
 * ============================================================================ */

/**
 * Write a 24-bit BMP image file
 */
void write_bmp(const char *filename, Pixel *img, int w, int h);

/* ============================================================================
 * FUNCTION DECLARATIONS - file_io.c
 * ============================================================================ */

/**
 * Load celestial bodies from file
 */
int load_bodies(const char *filename, Body *bodies, int max_bodies);

/**
 * Load rockets from file
 */
int load_rockets(const char *filename, Rocket *rockets, int max_rockets);

/**
 * Load simulation configuration
 */
int load_config(const char *filename, SimConfig *config);

/**
 * Save final rocket data
 */
void save_rocket_data(const char *filename, Rocket *rockets, int n);

/**
 * Save rocket trajectories in binary format
 */
void save_rocket_trails_bin(const char *filename, Rocket *rockets, int n);

/**
 * Save trajectory statistics
 */
void save_trajectory_stats(const char *filename, Rocket *rockets, int n, double dt);

/**
 * Save simulation metadata
 */
void save_metadata(const char *filename, int n_bodies, int n_rockets, SimConfig *config);

/* ============================================================================
 * FUNCTION DECLARATIONS - init.c
 * ============================================================================ */

/**
 * Initialize bodies with default configuration
 */
void init_bodies_default(Body *bodies, int *n);

/**
 * Initialize rockets with default configuration
 */
void init_rockets_default(Rocket *rockets, int *n);

/* ============================================================================
 * FUNCTION DECLARATIONS - physics.c
 * ============================================================================ */

/**
 * Compute gravitational forces between bodies
 */
void compute_forces(Body *bodies, int n, double g);

/**
 * Compute gravitational forces on rockets
 */
void compute_rocket_forces(Rocket *rockets, int n_rockets, 
                          Body *bodies, int n_bodies, double g);

/**
 * Update body positions and velocities
 */
void update_bodies(Body *bodies, int n, double dt, double g);

/**
 * Update rocket positions and velocities
 */
void update_rockets(Rocket *rockets, int n_rockets, Body *bodies, 
                   int n_bodies, double dt, double g);

/* ============================================================================
 * FUNCTION DECLARATIONS - render.c
 * ============================================================================ */

/**
 * Render the current simulation state
 */
void render(Body *bodies, int n_bodies, Rocket *rockets, int n_rockets, 
            Pixel *img, double scale);

#endif /* NBODY_H */