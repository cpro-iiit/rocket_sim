/**
 * physics.c - Physics Simulation Functions
 * Implements gravitational force calculations and numerical integration
 */

#include "nbody.h"

/**
 * Compute gravitational forces between all bodies using Newton's law
 * F = G * m1 * m2 / r^2
 * 
 * Uses softening parameter to prevent numerical singularities
 */
void compute_forces(Body *bodies, int n, double g) {
    // Reset accelerations
    for (int i = 0; i < n; i++) {
        bodies[i].ax = 0.0;
        bodies[i].ay = 0.0;
    }
    
    // Compute pairwise forces (Newton's third law)
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Vector from body i to body j
            double dx = bodies[j].x - bodies[i].x;
            double dy = bodies[j].y - bodies[i].y;
            
            // Distance squared with softening
            double dist_sq = dx * dx + dy * dy + SOFTENING * SOFTENING;
            double dist = sqrt(dist_sq);
            
            // Force magnitude: F = G / r^2
            double force = g / dist_sq;
            
            // Force components (unit vector * magnitude)
            double fx = force * dx / dist;
            double fy = force * dy / dist;
            
            // Apply forces (Newton's third law: equal and opposite)
            bodies[i].ax += fx * bodies[j].mass;
            bodies[i].ay += fy * bodies[j].mass;
            bodies[j].ax -= fx * bodies[i].mass;
            bodies[j].ay -= fy * bodies[i].mass;
        }
    }
}

/**
 * Compute gravitational acceleration on rockets from all bodies
 * Rockets don't exert forces, only experience them (test particles)
 */
void compute_rocket_forces(Rocket *rockets, int n_rockets, 
                          Body *bodies, int n_bodies, double g) {
    for (int i = 0; i < n_rockets; i++) {
        if (!rockets[i].active) continue;
        
        // Reset acceleration
        rockets[i].ax = 0.0;
        rockets[i].ay = 0.0;
        
        // Sum gravitational acceleration from all bodies
        for (int j = 0; j < n_bodies; j++) {
            double dx = bodies[j].x - rockets[i].x;
            double dy = bodies[j].y - rockets[i].y;
            
            double dist_sq = dx * dx + dy * dy + SOFTENING * SOFTENING;
            double dist = sqrt(dist_sq);
            
            // Acceleration = G * M / r^2
            double acc = g * bodies[j].mass / dist_sq;
            
            rockets[i].ax += acc * dx / dist;
            rockets[i].ay += acc * dy / dist;
        }
    }
}

/**
 * Update body positions and velocities using leapfrog integration
 * This symplectic integrator conserves energy better than Euler method
 * 
 * Steps:
 * 1. Compute accelerations from forces
 * 2. Update velocities: v(t+dt) = v(t) + a(t) * dt
 * 3. Update positions: x(t+dt) = x(t) + v(t+dt) * dt
 */
void update_bodies(Body *bodies, int n, double dt, double g) {
    // Compute accelerations
    compute_forces(bodies, n, g);
    
    // Integrate equations of motion
    for (int i = 0; i < n; i++) {
        // Update velocity
        bodies[i].vx += bodies[i].ax * dt;
        bodies[i].vy += bodies[i].ay * dt;
        
        // Update position
        bodies[i].x += bodies[i].vx * dt;
        bodies[i].y += bodies[i].vy * dt;
    }
}

/**
 * Update rocket positions and velocities
 * Same integration scheme as bodies, with trajectory recording
 */
void update_rockets(Rocket *rockets, int n_rockets, Body *bodies, 
                   int n_bodies, double dt, double g) {
    // Compute accelerations
    compute_rocket_forces(rockets, n_rockets, bodies, n_bodies, g);
    
    // Integrate equations of motion
    for (int i = 0; i < n_rockets; i++) {
        if (!rockets[i].active) continue;
        
        // Update velocity
        rockets[i].vx += rockets[i].ax * dt;
        rockets[i].vy += rockets[i].ay * dt;
        
        // Update position
        rockets[i].x += rockets[i].vx * dt;
        rockets[i].y += rockets[i].vy * dt;
        
        // Store position in trail for visualization
        if (rockets[i].trail_length < rockets[i].trail_capacity) {
            rockets[i].trail_x[rockets[i].trail_length] = rockets[i].x;
            rockets[i].trail_y[rockets[i].trail_length] = rockets[i].y;
            rockets[i].trail_length++;
        }
        
        // Deactivate rocket if it goes too far
        double dist = sqrt(rockets[i].x * rockets[i].x + 
                          rockets[i].y * rockets[i].y);
        if (dist > 50.0) {
            rockets[i].active = 0;
            printf("Rocket %d left simulation area (distance: %.2f)\n", i, dist);
        }
    }
}