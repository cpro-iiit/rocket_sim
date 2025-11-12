/**
 * test_file_io.c - File I/O Test Suite
 * 
 * Tests configuration parsing, body/rocket loading,
 * and binary file operations.
 */

#include "nbody.h"
#include <assert.h>

#define TEST_PASS "\033[32m✓ PASS\033[0m"
#define TEST_FAIL "\033[31m✗ FAIL\033[0m"
#define TEST_DIR "test/test_data/"

int tests_run = 0;
int tests_passed = 0;

void test_result(const char *test_name, int passed) {
    tests_run++;
    if (passed) {
        tests_passed++;
        printf("  %s: %s\n", TEST_PASS, test_name);
    } else {
        printf("  %s: %s\n", TEST_FAIL, test_name);
    }
}

/**
 * Create test data files
 */
void create_test_files() {
    // Create test directory if it doesn't exist
    system("mkdir -p " TEST_DIR);
    
    // Create test_bodies.txt
    FILE *f = fopen(TEST_DIR "test_bodies.txt", "w");
    if (f) {
        fprintf(f, "# Test bodies file\n");
        fprintf(f, "0.0 0.0 0.0 0.0 100.0\n");
        fprintf(f, "2.0 0.0 0.0 0.707 5.0\n");
        fprintf(f, "-2.0 0.0 0.0 -0.707 5.0\n");
        fclose(f);
    }
    
    // Create test_rockets.txt
    f = fopen(TEST_DIR "test_rockets.txt", "w");
    if (f) {
        fprintf(f, "# Test rockets file\n");
        fprintf(f, "2.0 0.0 0.0 1.732\n");
        fprintf(f, "-3.0 0.0 0.0 -1.414\n");
        fclose(f);
    }
    
    // Create test_config.txt
    f = fopen(TEST_DIR "test_config.txt", "w");
    if (f) {
        fprintf(f, "# Test configuration\n");
        fprintf(f, "dt=0.01\n");
        fprintf(f, "steps=1000\n");
        fprintf(f, "frames=50\n");
        fprintf(f, "save_interval=20\n");
        fprintf(f, "g=1.5\n");
        fclose(f);
    }
}

/**
 * Test 1: Load bodies from file
 */
void test_load_bodies() {
    Body bodies[MAX_BODIES];
    int n = load_bodies(TEST_DIR "test_bodies.txt", bodies, MAX_BODIES);
    
    int passed = (n == 3) && 
                 (bodies[0].mass == 100.0) &&
                 (bodies[1].x == 2.0) &&
                 (bodies[2].x == -2.0);
    
    test_result("Load bodies from file", passed);
}

/**
 * Test 2: Load rockets from file
 */
void test_load_rockets() {
    Rocket rockets[MAX_ROCKETS];
    int n = load_rockets(TEST_DIR "test_rockets.txt", rockets, MAX_ROCKETS);
    
    int passed = (n == 2) &&
                 (rockets[0].x == 2.0) &&
                 (rockets[1].x == -3.0) &&
                 (rockets[0].trail_x != NULL) &&
                 (rockets[1].trail_x != NULL);
    
    // Clean up
    for (int i = 0; i < n; i++) {
        free(rockets[i].trail_x);
        free(rockets[i].trail_y);
    }
    
    test_result("Load rockets from file", passed);
}

/**
 * Test 3: Load configuration
 */
void test_load_config() {
    SimConfig config = {DT, STEPS, FRAMES, 0, G};
    int result = load_config(TEST_DIR "test_config.txt", &config);
    
    int passed = (result == 0) &&
                 (config.dt == 0.01) &&
                 (config.steps == 1000) &&
                 (config.frames == 50) &&
                 (config.save_interval == 20) &&
                 (config.g == 1.5);
    
    test_result("Load configuration", passed);
}

/**
 * Test 4: Handle missing files gracefully
 */
void test_missing_files() {
    Body bodies[MAX_BODIES];
    SimConfig config = {DT, STEPS, FRAMES, 0, G};
    
    int bodies_result = load_bodies("nonexistent.txt", bodies, MAX_BODIES);
    int config_result = load_config("nonexistent.txt", &config);
    
    // Should return -1 for missing files, not crash
    int passed = (bodies_result == -1) && (config_result == -1);
    
    test_result("Handle missing files", passed);
}

/**
 * Test 5: Save and load binary trajectory
 */
void test_binary_trajectory() {
    // Create test rocket with trail
    Rocket rockets[1];
    rockets[0].x = 5.0;
    rockets[0].y = 3.0;
    rockets[0].vx = 1.0;
    rockets[0].vy = 2.0;
    rockets[0].active = 1;
    rockets[0].trail_length = 100;
    rockets[0].trail_capacity = 100;
    rockets[0].trail_x = (double *)malloc(100 * sizeof(double));
    rockets[0].trail_y = (double *)malloc(100 * sizeof(double));
    
    // Fill trail with test data
    for (int i = 0; i < 100; i++) {
        rockets[0].trail_x[i] = i * 0.1;
        rockets[0].trail_y[i] = i * 0.2;
    }
    
    // Save to binary file
    save_rocket_trails_bin(TEST_DIR "test_trails.bin", rockets, 1);
    
    // Read back and verify
    FILE *f = fopen(TEST_DIR "test_trails.bin", "rb");
    int passed = 0;
    
    if (f) {
        int n_rockets;
        fread(&n_rockets, sizeof(int), 1, f);
        
        int trail_length;
        fread(&trail_length, sizeof(int), 1, f);
        
        double *read_x = (double *)malloc(trail_length * sizeof(double));
        double *read_y = (double *)malloc(trail_length * sizeof(double));
        
        fread(read_x, sizeof(double), trail_length, f);
        fread(read_y, sizeof(double), trail_length, f);
        
        passed = (n_rockets == 1) &&
                 (trail_length == 100) &&
                 (read_x[0] == 0.0) &&
                 (read_y[99] == 19.8);
        
        free(read_x);
        free(read_y);
        fclose(f);
    }
    
    free(rockets[0].trail_x);
    free(rockets[0].trail_y);
    
    test_result("Binary trajectory save/load", passed);
}

/**
 * Test 6: Save rocket data text format
 */
void test_save_rocket_data() {
    Rocket rockets[2];
    
    rockets[0].x = 1.5;
    rockets[0].y = 2.5;
    rockets[0].vx = 0.5;
    rockets[0].vy = 1.5;
    rockets[0].trail_length = 100;
    
    rockets[1].x = -1.5;
    rockets[1].y = -2.5;
    rockets[1].vx = -0.5;
    rockets[1].vy = -1.5;
    rockets[1].trail_length = 200;
    
    save_rocket_data(TEST_DIR "test_final.txt", rockets, 2);
    
    // Read back and verify
    FILE *f = fopen(TEST_DIR "test_final.txt", "r");
    int passed = 0;
    
    if (f) {
        char line[256];
        fgets(line, sizeof(line), f);  // Skip header
        
        int id;
        double x, y, vx, vy;
        int trail_len;
        
        fscanf(f, "%d %lf %lf %lf %lf %d", &id, &x, &y, &vx, &vy, &trail_len);
        
        passed = (id == 0) && (x == 1.5) && (trail_len == 100);
        
        fclose(f);
    }
    
    test_result("Save rocket data (text)", passed);
}

/**
 * Test 7: Comment handling in config files
 */
void test_comment_handling() {
    // Create file with comments
    FILE *f = fopen(TEST_DIR "test_comments.txt", "w");
    if (f) {
        fprintf(f, "# This is a comment\n");
        fprintf(f, "0.0 0.0 0.0 0.0 50.0\n");
        fprintf(f, "# Another comment\n");
        fprintf(f, "\n");  // Empty line
        fprintf(f, "1.0 0.0 0.0 1.0 10.0\n");
        fclose(f);
    }
    
    Body bodies[MAX_BODIES];
    int n = load_bodies(TEST_DIR "test_comments.txt", bodies, MAX_BODIES);
    
    int passed = (n == 2) && (bodies[0].mass == 50.0) && (bodies[1].x == 1.0);
    
    test_result("Comment and whitespace handling", passed);
}

/**
 * Test 8: Configuration parameter validation
 */
void test_config_validation() {
    // Create config with various parameter formats
    FILE *f = fopen(TEST_DIR "test_params.txt", "w");
    if (f) {
        fprintf(f, "dt=0.005\n");
        fprintf(f, "steps=2000\n");
        fprintf(f, "frames=100\n");
        fprintf(f, "save_interval=40\n");
        fprintf(f, "g=0.5\n");
        fclose(f);
    }
    
    SimConfig config = {0};
    load_config(TEST_DIR "test_params.txt", &config);
    
    int passed = (config.dt == 0.005) &&
                 (config.steps == 2000) &&
                 (config.g == 0.5);
    
    test_result("Configuration parameter parsing", passed);
}

int main() {
    printf("\n");
    printf("====================================\n");
    printf("File I/O Test Suite\n");
    printf("====================================\n\n");
    
    // Create test data files
    create_test_files();
    
    // Run tests
    test_load_bodies();
    test_load_rockets();
    test_load_config();
    test_missing_files();
    test_binary_trajectory();
    test_save_rocket_data();
    test_comment_handling();
    test_config_validation();
    
    printf("\n====================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);
    printf("====================================\n\n");
    
    return (tests_passed == tests_run) ? 0 : 1;
}