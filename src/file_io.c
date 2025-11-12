/**
 * file_io.c - File I/O Operations
 * Handles loading and saving simulation data
 */

#include "nbody.h"

/**
 * Load Body Parameters from a File (Exercise 1.1)
 */
int load_bodies(const char *filename, Body *bodies, int max_bodies) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Warning: Could not open %s, using default bodies\n", filename);
        return -1;
    }
    
    int count = 0;
    char line[256];
    
    printf("Loading bodies from %s...\n", filename);
    
    while (fgets(line, sizeof(line), f) && count < max_bodies) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        double x, y, vx, vy, mass;
        if (sscanf(line, "%lf %lf %lf %lf %lf", &x, &y, &vx, &vy, &mass) == 5) {
            bodies[count].x = x;
            bodies[count].y = y;
            bodies[count].vx = vx;
            bodies[count].vy = vy;
            bodies[count].mass = mass;
            bodies[count].ax = 0.0;
            bodies[count].ay = 0.0;
            
            printf("  Body %d: pos(%.2f, %.2f) vel(%.2f, %.2f) mass=%.2f\n",
                   count, x, y, vx, vy, mass);
            count++;
        }
    }
    
    fclose(f);
    printf("Loaded %d bodies\n\n", count);
    return count;
}

/**
 * Initialize Rockets from a File (Exercise 1.2)
 */
int load_rockets(const char *filename, Rocket *rockets, int max_rockets) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Warning: Could not open %s, using default rockets\n", filename);
        return -1;
    }
    
    int count = 0;
    char line[256];
    
    printf("Loading rockets from %s...\n", filename);
    
    while (fgets(line, sizeof(line), f) && count < max_rockets) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        double x, y, vx, vy;
        if (sscanf(line, "%lf %lf %lf %lf", &x, &y, &vx, &vy) == 4) {
            rockets[count].x = x;
            rockets[count].y = y;
            rockets[count].vx = vx;
            rockets[count].vy = vy;
            rockets[count].ax = 0.0;
            rockets[count].ay = 0.0;
            rockets[count].active = 1;
            
            rockets[count].trail_capacity = STEPS;
            rockets[count].trail_x = (double *)malloc(STEPS * sizeof(double));
            rockets[count].trail_y = (double *)malloc(STEPS * sizeof(double));
            rockets[count].trail_length = 0;
            
            rockets[count].trail_x[0] = x;
            rockets[count].trail_y[0] = y;
            rockets[count].trail_length = 1;
            
            printf("  Rocket %d: pos(%.2f, %.2f) vel(%.4f, %.4f)\n",
                   count, x, y, vx, vy);
            count++;
        }
    }
    
    fclose(f);
    printf("Loaded %d rockets\n\n", count);
    return count;
}

/**
 * Load Simulation Configuration (Exercise 4.3)
 */
int load_config(const char *filename, SimConfig *config) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Warning: Could not open %s, using default config\n", filename);
        return -1;
    }
    
    char line[256];
    printf("Loading configuration from %s...\n", filename);
    
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        char key[64];
        double value;
        if (sscanf(line, "%[^=]=%lf", key, &value) == 2) {
            if (strcmp(key, "dt") == 0) config->dt = value;
            else if (strcmp(key, "steps") == 0) config->steps = (int)value;
            else if (strcmp(key, "frames") == 0) config->frames = (int)value;
            else if (strcmp(key, "save_interval") == 0) config->save_interval = (int)value;
            else if (strcmp(key, "g") == 0) config->g = value;
        }
    }
    
    fclose(f);
    printf("Configuration loaded:\n");
    printf("  dt=%.4f, steps=%d, frames=%d, save_interval=%d, g=%.2f\n\n",
           config->dt, config->steps, config->frames, 
           config->save_interval, config->g);
    return 0;
}

/**
 * Save Final Rocket Positions (Exercise 2.1)
 */
void save_rocket_data(const char *filename, Rocket *rockets, int n) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error: Could not create %s\n", filename);
        return;
    }
    
    fprintf(f, "# Rocket_ID   Final_X   Final_Y   Final_VX   Final_VY   Trail_Length\n");
    
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d   %.6f   %.6f   %.6f   %.6f   %d\n",
                i, rockets[i].x, rockets[i].y, 
                rockets[i].vx, rockets[i].vy,
                rockets[i].trail_length);
    }
    
    fclose(f);
    printf("Saved final rocket data to %s\n", filename);
}

/**
 * Save Rocket Trajectories in Binary Format (Exercise 2.2)
 */
void save_rocket_trails_bin(const char *filename, Rocket *rockets, int n) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("Error: Could not create %s\n", filename);
        return;
    }
    
    fwrite(&n, sizeof(int), 1, f);
    
    for (int i = 0; i < n; i++) {
        fwrite(&rockets[i].trail_length, sizeof(int), 1, f);
        fwrite(rockets[i].trail_x, sizeof(double), rockets[i].trail_length, f);
        fwrite(rockets[i].trail_y, sizeof(double), rockets[i].trail_length, f);
    }
    
    fclose(f);
    printf("Saved rocket trails in binary format to %s\n", filename);
}

/**
 * Save Trajectory Summary Statistics (Exercise 3.1)
 */
void save_trajectory_stats(const char *filename, Rocket *rockets, int n, double dt) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error: Could not create %s\n", filename);
        return;
    }
    
    fprintf(f, "RocketID,TrailLength,FinalDistance,MaxDistance,AverageSpeed\n");
    
    for (int i = 0; i < n; i++) {
        double final_dist = sqrt(rockets[i].x * rockets[i].x + 
                                rockets[i].y * rockets[i].y);
        
        double max_dist = 0.0;
        double total_distance = 0.0;
        
        for (int j = 0; j < rockets[i].trail_length; j++) {
            double dist = sqrt(rockets[i].trail_x[j] * rockets[i].trail_x[j] +
                             rockets[i].trail_y[j] * rockets[i].trail_y[j]);
            if (dist > max_dist) max_dist = dist;
            
            if (j > 0) {
                double dx = rockets[i].trail_x[j] - rockets[i].trail_x[j-1];
                double dy = rockets[i].trail_y[j] - rockets[i].trail_y[j-1];
                total_distance += sqrt(dx * dx + dy * dy);
            }
        }
        
        double sim_time = rockets[i].trail_length * dt;
        double avg_speed = (sim_time > 0) ? total_distance / sim_time : 0.0;
        
        fprintf(f, "%d,%d,%.3f,%.3f,%.6f\n",
                i, rockets[i].trail_length, final_dist, max_dist, avg_speed);
    }
    
    fclose(f);
    printf("Saved trajectory statistics to %s\n", filename);
}

/**
 * Save Simulation Metadata (Exercise 4.2)
 */
void save_metadata(const char *filename, int n_bodies, int n_rockets, 
                   SimConfig *config) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Error: Could not create %s\n", filename);
        return;
    }
    
    time_t now = time(NULL);
    fprintf(f, "# Simulation Metadata\n");
    fprintf(f, "# Generated: %s\n", ctime(&now));
    fprintf(f, "Width=%d\n", WIDTH);
    fprintf(f, "Height=%d\n", HEIGHT);
    fprintf(f, "Steps=%d\n", config->steps);
    fprintf(f, "DT=%.6f\n", config->dt);
    fprintf(f, "G=%.6f\n", config->g);
    fprintf(f, "N_Bodies=%d\n", n_bodies);
    fprintf(f, "N_Rockets=%d\n", n_rockets);
    fprintf(f, "Frames=%d\n", config->frames);
    fprintf(f, "Save_Interval=%d\n", config->save_interval);
    fprintf(f, "Softening=%.6f\n", SOFTENING);
    
    fclose(f);
    printf("Saved simulation metadata to %s\n", filename);
}