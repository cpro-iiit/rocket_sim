/**
 * init.c - Initialization Functions
 * Default initialization for bodies and rockets
 */

#include "nbody.h"

/**
 * Initialize celestial bodies with default configuration
 * Creates a central massive star with orbiting planets
 */
void init_bodies_default(Body *bodies, int *n) {
    // Central massive body (star)
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].vx = 0.0;
    bodies[0].vy = 0.0;
    bodies[0].mass = 100.0;
    bodies[0].ax = 0.0;
    bodies[0].ay = 0.0;
    
    // Create orbiting bodies (planets)
    int count = 5;
    for (int i = 1; i < count; i++) {
        double angle = 2.0 * M_PI * i / (count - 1);
        double radius = 2.0 + i * 0.5;
        
        bodies[i].x = radius * cos(angle);
        bodies[i].y = radius * sin(angle);
        
        // Circular orbital velocity: v = sqrt(GM/r)
        double v = sqrt(G * bodies[0].mass / radius);
        bodies[i].vx = -v * sin(angle);
        bodies[i].vy = v * cos(angle);
        bodies[i].mass = 1.0;
        bodies[i].ax = 0.0;
        bodies[i].ay = 0.0;
    }
    
    *n = count;
    printf("Initialized %d default bodies\n", count);
}

/**
 * Initialize rockets with default elliptical orbit configuration
 * Creates a rocket in a stable elliptical orbit around the central mass
 */
void init_rockets_default(Rocket *rockets, int *n) {
    // Elliptical orbit parameters
    double semi_major = 5.0;        // Semi-major axis
    double eccentricity = 0.6;      // Eccentricity (0 = circle, <1 = ellipse)
    
    // Start at perihelion (closest point)
    double r = semi_major * (1.0 - eccentricity);
    
    // Central mass
    double M_central = 100.0;
    
    // Velocity at perihelion for elliptical orbit
    double v_perihelion = sqrt(G * M_central * (1.0 + eccentricity) / 
                               (semi_major * (1.0 - eccentricity)));
    
    // Initialize rocket
    rockets[0].x = r;
    rockets[0].y = 0.0;
    rockets[0].vx = 0.0;
    rockets[0].vy = v_perihelion;
    rockets[0].ax = 0.0;
    rockets[0].ay = 0.0;
    rockets[0].active = 1;
    
    // Allocate trail memory
    rockets[0].trail_capacity = STEPS;
    rockets[0].trail_x = (double *)malloc(STEPS * sizeof(double));
    rockets[0].trail_y = (double *)malloc(STEPS * sizeof(double));
    rockets[0].trail_length = 0;
    
    // Store initial position
    rockets[0].trail_x[0] = rockets[0].x;
    rockets[0].trail_y[0] = rockets[0].y;
    rockets[0].trail_length = 1;
    
    *n = 1;
    printf("Initialized %d default rocket(s) in elliptical orbit\n", *n);
    printf("  Semi-major axis: %.2f, Eccentricity: %.2f\n", semi_major, eccentricity);
    printf("  Perihelion: %.2f, Aphelion: %.2f\n", 
           r, semi_major * (1.0 + eccentricity));
}