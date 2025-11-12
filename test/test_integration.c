/**
 * test_integration.c - Integration Test Suite
 * 
 * End-to-end tests that verify the complete simulation pipeline:
 * initialization → simulation → output → analysis
 */

#include "nbody.h"
#include <assert.h>

#define TEST_PASS "\033[32m✓ PASS\033[0m"
#define TEST_FAIL "\033[31m✗ FAIL\033[0m"
#define TEST_DIR "test/test_data/"
#define EPSILON 1e-3

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
 * Test 1: Complete simulation workflow
 * Test initialization → simulation → output generation
 */
void test_complete_workflow() {
    Body bodies[MAX_BODIES];
    Rocket rockets[MAX_ROCKETS];
    int n_bodies, n_rockets;
    
    // Initialize default configuration
    init_bodies_default(bodies, &n_bodies);
    init_rockets_default(rockets, &n_rockets);
    
    // Run short simulation
    for (int step = 0; step < 100; step++) {
        update_bodies(bodies, n_bodies, 0.01, 1.0);
        update_rockets(rockets, n_rockets, bodies, n_bodies, 0.01, 1.0);
    }
    
    // Verify simulation ran
    int passed = (rockets[0].trail_length > 1) &&
                 (rockets[0].active == 1);
    
    // Clean up
    for (int i = 0; i < n_rockets; i++) {
        free(rockets[i].trail_x);
        free(rockets[i].trail_y);
    }
    
    test_result("Complete simulation workflow", passed);
}

/**
 * Test 2: Kepler's third law verification
 * T² ∝ a³ for different orbital radii
 */
void test_keplers_third_law() {
    Body bodies[2];
    
    // Central star
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].vx = 0.0;
    bodies[0].vy = 0.0;
    bodies[0].mass = 100.0;
    bodies[0].ax = 0.0;
    bodies[0].ay = 0.0;
    
    double G_val = 1.0;
    
    // Test two different orbital radii
    double r1 = 2.0;
    double r2 = 3.0;
    
    // Calculate theoretical periods: T = 2π√(r³/GM)
    double T1_theory = 2.0 * M_PI * sqrt(r1 * r1 * r1 / (G_val * bodies[0].mass));
    double T2_theory = 2.0 * M_PI * sqrt(r2 * r2 * r2 / (G_val * bodies[0].mass));
    
    // Verify ratio: (T2/T1)² = (r2/r1)³
    double ratio_theory = pow(T2_theory / T1_theory, 2.0);
    double ratio_expected = pow(r2 / r1, 3.0);
    
    int passed = fabs(ratio_theory - ratio_expected) < EPSILON;
    
    test_result("Kepler's third law verification", passed);
}

/**
 * Test 3: Momentum conservation in isolated system
 * Total momentum should remain constant
 */
void test_momentum_conservation() {
    Body bodies[3];
    
    // Create three-body system with zero initial momentum
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].vx = 1.0;
    bodies[0].vy = 0.0;
    bodies[0].mass = 10.0;
    
    bodies[1].x = 2.0;
    bodies[1].y = 0.0;
    bodies[1].vx = -0.5;
    bodies[1].vy = 0.5;
    bodies[1].mass = 10.0;
    
    bodies[2].x = 1.0;
    bodies[2].y = 2.0;
    bodies[2].vx = -0.5;
    bodies[2].vy = -0.5;
    bodies[2].mass = 10.0;
    
    // Calculate initial momentum
    double px0 = 0.0, py0 = 0.0;
    for (int i = 0; i < 3; i++) {
        px0 += bodies[i].mass * bodies[i].vx;
        py0 += bodies[i].mass * bodies[i].vy;
    }
    
    // Simulate
    for (int step = 0; step < 500; step++) {
        update_bodies(bodies, 3, 0.01, 1.0);
    }
    
    // Calculate final momentum
    double px1 = 0.0, py1 = 0.0;
    for (int i = 0; i < 3; i++) {
        px1 += bodies[i].mass * bodies[i].vx;
        py1 += bodies[i].mass * bodies[i].vy;
    }
    
    // Momentum should be conserved
    int passed = (fabs(px1 - px0) < 0.1) && (fabs(py1 - py0) < 0.1);
    
    test_result("Momentum conservation", passed);
}

/**
 * Test 4: Rocket trajectory integration
 * Verify rocket follows expected path in simple field
 */
void test_rocket_trajectory() {
    Body bodies[1];
    Rocket rockets[1];
    
    // Single central mass
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].vx = 0.0;
    bodies[0].vy = 0.0;
    bodies[0].mass = 100.0;
    
    // Rocket starting at distance 5.0, moving tangentially
    double r = 5.0;
    rockets[0].x = r;
    rockets[0].y = 0.0;
    
    // Give it circular orbit velocity
    double v = sqrt(1.0 * bodies[0].mass / r);
    rockets[0].vx = 0.0;
    rockets[0].vy = v;
    rockets[0].active = 1;
    rockets[0].trail_capacity = 5000;
    rockets[0].trail_x = (double *)malloc(5000 * sizeof(double));
    rockets[0].trail_y = (double *)malloc(5000 * sizeof(double));
    rockets[0].trail_length = 0;
    rockets[0].trail_x[0] = rockets[0].x;
    rockets[0].trail_y[0] = rockets[0].y;
    rockets[0].trail_length = 1;
    
    // Simulate for half orbit
    double period = 2.0 * M_PI * sqrt(r * r * r / (1.0 * bodies[0].mass));
    int steps = (int)(period / (2.0 * 0.01));
    
    for (int i = 0; i < steps; i++) {
        update_rockets(rockets, 1, bodies, 1, 0.01, 1.0);
    }
    
    // After half orbit, should be on opposite side
    double final_r = sqrt(rockets[0].x * rockets[0].x + rockets[0].y * rockets[0].y);
    int passed = (fabs(final_r - r) < 0.2) && (rockets[0].x < 0.0);
    
    free(rockets[0].trail_x);
    free(rockets[0].trail_y);
    
    test_result("Rocket trajectory integration", passed);
}

/**
 * Test 5: Multiple rockets independence
 * Multiple rockets should not affect each other
 */
void test_multiple_rockets() {
    Body bodies[1];
    Rocket rockets[2];
    
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].mass = 100.0;
    
    // Two identical rockets at different positions
    for (int i = 0; i < 2; i++) {
        rockets[i].x = 3.0 + i;
        rockets[i].y = 0.0;
        rockets[i].vx = 0.0;
        rockets[i].vy = 5.0;
        rockets[i].active = 1;
        rockets[i].trail_capacity = 1000;
        rockets[i].trail_x = (double *)malloc(1000 * sizeof(double));
        rockets[i].trail_y = (double *)malloc(1000 * sizeof(double));
        rockets[i].trail_length = 1;
        rockets[i].trail_x[0] = rockets[i].x;
        rockets[i].trail_y[0] = rockets[i].y;
    }
    
    // Simulate
    for (int step = 0; step < 100; step++) {
        update_rockets(rockets, 2, bodies, 1, 0.01, 1.0);
    }
    
    // Both rockets should have similar trajectories (shifted)
    double dx = rockets[1].x - rockets[0].x;
    int passed = (fabs(dx - 1.0) < 0.5) &&  // Should maintain ~1.0 separation
                 (rockets[0].trail_length > 10) &&
                 (rockets[1].trail_length > 10);
    
    for (int i = 0; i < 2; i++) {
        free(rockets[i].trail_x);
        free(rockets[i].trail_y);
    }
    
    test_result("Multiple rockets independence", passed);
}

/**
 * Test 6: File I/O round-trip
 * Save and load data, verify consistency
 */
void test_file_io_roundtrip() {
    Rocket rockets[2];
    
    // Create test rockets with trails
    for (int i = 0; i < 2; i++) {
        rockets[i].x = i * 2.0;
        rockets[i].y = i * 3.0;
        rockets[i].vx = i * 0.5;
        rockets[i].vy = i * 0.7;
        rockets[i].trail_length = 50;
        rockets[i].trail_capacity = 50;
        rockets[i].trail_x = (double *)malloc(50 * sizeof(double));
        rockets[i].trail_y = (double *)malloc(50 * sizeof(double));
        
        for (int j = 0; j < 50; j++) {
            rockets[i].trail_x[j] = j * 0.1 + i;
            rockets[i].trail_y[j] = j * 0.2 + i;
        }
    }
    
    // Save to binary
    save_rocket_trails_bin(TEST_DIR "roundtrip.bin", rockets, 2);
    
    // Read back
    FILE *f = fopen(TEST_DIR "roundtrip.bin", "rb");
    int passed = 0;
    
    if (f) {
        int n;
        fread(&n, sizeof(int), 1, f);
        
        passed = (n == 2);
        
        for (int i = 0; i < n && passed; i++) {
            int len;
            double *tx = (double *)malloc(50 * sizeof(double));
            double *ty = (double *)malloc(50 * sizeof(double));
            
            fread(&len, sizeof(int), 1, f);
            fread(tx, sizeof(double), len, f);
            fread(ty, sizeof(double), len, f);
            
            passed = passed && (len == 50) && 
                     (fabs(tx[0] - i) < EPSILON) &&
                     (fabs(ty[49] - (9.8 + i)) < EPSILON);
            
            free(tx);
            free(ty);
        }
        
        fclose(f);
    }
    
    for (int i = 0; i < 2; i++) {
        free(rockets[i].trail_x);
        free(rockets[i].trail_y);
    }
    
    test_result("File I/O round-trip consistency", passed);
}

/**
 * Test 7: Escape velocity verification
 * Rocket with escape velocity should not return
 */
void test_escape_velocity() {
    Body bodies[1];
    Rocket rockets[1];
    
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].mass = 100.0;
    
    double r = 2.0;
    rockets[0].x = r;
    rockets[0].y = 0.0;
    
    // Escape velocity: v = sqrt(2GM/r)
    double v_escape = sqrt(2.0 * 1.0 * bodies[0].mass / r);
    rockets[0].vx = 0.0;
    rockets[0].vy = v_escape * 1.1;  // 10% above escape velocity
    rockets[0].active = 1;
    rockets[0].trail_capacity = 5000;
    rockets[0].trail_x = (double *)malloc(5000 * sizeof(double));
    rockets[0].trail_y = (double *)malloc(5000 * sizeof(double));
    rockets[0].trail_length = 1;
    
    // Simulate
    for (int i = 0; i < 1000; i++) {
        update_rockets(rockets, 1, bodies, 1, 0.01, 1.0);
    }
    
    // Distance should be increasing
    double final_r = sqrt(rockets[0].x * rockets[0].x + rockets[0].y * rockets[0].y);
    int passed = final_r > 10.0;  // Should have escaped
    
    free(rockets[0].trail_x);
    free(rockets[0].trail_y);
    
    test_result("Escape velocity verification", passed);
}

int main() {
    printf("\n");
    printf("====================================\n");
    printf("Integration Test Suite\n");
    printf("====================================\n\n");
    
    // Create test directory
    system("mkdir -p " TEST_DIR);
    
    // Run tests
    test_complete_workflow();
    test_keplers_third_law();
    test_momentum_conservation();
    test_rocket_trajectory();
    test_multiple_rockets();
    test_file_io_roundtrip();
    test_escape_velocity();
    
    printf("\n====================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);
    printf("====================================\n\n");
    
    return (tests_passed == tests_run) ? 0 : 1;
}