/**
 * main.c - Main Program Entry Point
 * N-Body Simulation with Rocket Trajectories
 */

#include "nbody.h"

int main() {
    Body bodies[MAX_BODIES];
    Rocket rockets[MAX_ROCKETS];
    Pixel *img = (Pixel *)malloc(WIDTH * HEIGHT * sizeof(Pixel));
    
    if (!img) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }
    
    // Load configuration
    SimConfig config = {DT, STEPS, FRAMES, STEPS/FRAMES, G};
    load_config("config.txt", &config);
    
    // User interaction (Exercise 4.1)
    printf("\n====================================\n");
    printf("N-Body Rocket Simulation\n");
    printf("====================================\n");
    printf("Choose mode:\n");
    printf("  [N] Run new simulation\n");
    printf("  [L] Load and plot saved data\n");
    printf("Choice (N/L): ");
    
    char choice;
    scanf(" %c", &choice);
    printf("\n");
    
    if (choice == 'L' || choice == 'l') {
        printf("Loading mode not yet fully implemented.\n");
        printf("Use plot_trails tool to visualize saved data.\n");
        printf("Falling back to new simulation...\n\n");
    }
    
    // Load or initialize bodies
    int n_bodies = load_bodies("bodies.txt", bodies, MAX_BODIES);
    if (n_bodies < 0) {
        init_bodies_default(bodies, &n_bodies);
    }
    
    // Load or initialize rockets
    int n_rockets = load_rockets("rockets.txt", rockets, MAX_ROCKETS);
    if (n_rockets < 0) {
        init_rockets_default(rockets, &n_rockets);
    }
    
    double scale = 50.0;
    int frame_interval = config.steps / config.frames;
    if (config.save_interval > 0) {
        frame_interval = config.save_interval;
    }
    
    printf("\n====================================\n");
    printf("Simulation Parameters\n");
    printf("====================================\n");
    printf("Bodies: %d\n", n_bodies);
    printf("Rockets: %d\n", n_rockets);
    printf("Steps: %d\n", config.steps);
    printf("Time step: %.4f\n", config.dt);
    printf("Frames: %d\n", config.frames);
    printf("Save interval: %d steps\n", frame_interval);
    printf("====================================\n\n");
    
    // Open frame log file (Exercise 2.3)
    FILE *log = fopen("frames.log", "w");
    if (log) {
        time_t now = time(NULL);
        fprintf(log, "# Frame Generation Log\n");
        fprintf(log, "# Started: %s\n", ctime(&now));
    }
    
    // Save metadata (Exercise 4.2)
    save_metadata("metadata.txt", n_bodies, n_rockets, &config);
    
    printf("Starting simulation...\n");
    
    // Main simulation loop
    for (int step = 0; step < config.steps; step++) {
        // Update physics for one time step
        update_bodies(bodies, n_bodies, config.dt, config.g);
        update_rockets(rockets, n_rockets, bodies, n_bodies, config.dt, config.g);
        
        // Generate output frame at intervals
        if (step % frame_interval == 0) {
            char filename[50];
            int frame_num = step / frame_interval;
            sprintf(filename, "frame_%04d.bmp", frame_num);
            
            // Render and save frame
            render(bodies, n_bodies, rockets, n_rockets, img, scale);
            write_bmp(filename, img, WIDTH, HEIGHT);
            
            // Log frame information
            if (log) {
                fprintf(log, "Frame %d: Step %d", frame_num, step);
                for (int i = 0; i < n_rockets; i++) {
                    if (rockets[i].active || rockets[i].trail_length > 0) {
                        fprintf(log, ", Rocket%d_Pos (%.2f, %.2f)", 
                               i, rockets[i].x, rockets[i].y);
                    }
                }
                fprintf(log, "\n");
            }
            
            printf("Step %5d/%d - Generated %s\n", step, config.steps, filename);
        }
    }
    
    // Close log file
    if (log) {
        fclose(log);
        printf("\nFrame log saved to frames.log\n");
    }
    
    printf("\n====================================\n");
    printf("Simulation Complete!\n");
    printf("====================================\n\n");
    
    // Save output files
    save_rocket_data("final_rockets.txt", rockets, n_rockets);
    save_rocket_trails_bin("rocket_trails.bin", rockets, n_rockets);
    save_trajectory_stats("rocket_stats.csv", rockets, n_rockets, config.dt);
    
    printf("\nOutput files generated:\n");
    printf("  - frame_XXXX.bmp (visualization frames)\n");
    printf("  - final_rockets.txt (final positions and velocities)\n");
    printf("  - rocket_trails.bin (binary trajectory data)\n");
    printf("  - rocket_stats.csv (statistical analysis)\n");
    printf("  - frames.log (frame generation log)\n");
    printf("  - metadata.txt (simulation parameters)\n");
    
    // Clean up
    free(img);
    for (int i = 0; i < n_rockets; i++) {
        free(rockets[i].trail_x);
        free(rockets[i].trail_y);
    }
    
    printf("\n====================================\n");
    printf("All done! Check output files.\n");
    printf("====================================\n\n");
    
    return 0;
}