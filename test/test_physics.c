/**
 * test_physics.c - Physics Module Test Suite
 * 
 * Tests gravitational force calculations, numerical integration,
 * and energy conservation properties.
 */

#include "nbody.h"
#include <assert.h>

#define EPSILON 1e-6
#define TEST_PASS "\033[32m✓ PASS\033[0m"
#define TEST_FAIL "\033[31m✗ FAIL\033[0m"

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
 * Test 1: Two-body force calculation
 * Verify Newton's law: F = G * m1 * m2 / r^2
 */
void test_two_body_force() {
    Body bodies[2];
    
    // Place two bodies along x-axis
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].mass = 10.0;
    
    bodies[1].x = 2.0;  // Distance = 2.0
    bodies[1].y = 0.0;
    bodies[1].mass = 5.0;
    
    compute_forces(bodies, 2, 1.0);  // G = 1.0
    
    // Expected force magnitude: F = G * m1 * m2 / r^2 = 1.0 * 10 * 5 / 4 = 12.5
    // Force on body 0: +12.5 in x direction (per unit mass of body 0)
    // Acceleration = F / m = 12.5 / 10 = 1.25
    
    double expected_ax = 1.25;
    int passed = fabs(bodies[0].ax - expected_ax) < EPSILON;
    
    test_result("Two-body force calculation", passed);
}

/**
 * Test 2: Force symmetry (Newton's third law)
 * Forces should be equal and opposite
 */
void test_force_symmetry() {
    Body bodies[2];
    
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].mass = 10.0;
    
    bodies[1].x = 3.0;
    bodies[1].y = 4.0;  // Distance = 5.0
    bodies[1].mass = 20.0;
    
    compute_forces(bodies, 2, 1.0);
    
    // Force magnitude should be equal
    double f0 = sqrt(bodies[0].ax * bodies[0].ax + bodies[0].ay * bodies[0].ay) * bodies[0].mass;
    double f1 = sqrt(bodies[1].ax * bodies[1].ax + bodies[1].ay * bodies[1].ay) * bodies[1].mass;
    
    int passed = fabs(f0 - f1) < EPSILON;
    
    test_result("Force symmetry (Newton's 3rd law)", passed);
}

/**
 * Test 3: Circular orbit stability
 * A body with correct orbital velocity should maintain circular orbit
 */
void test_circular_orbit() {
    Body bodies[2];
    
    // Central mass
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].vx = 0.0;
    bodies[0].vy = 0.0;
    bodies[0].mass = 100.0;
    
    // Orbiting body at distance 2.0
    double r = 2.0;
    bodies[1].x = r;
    bodies[1].y = 0.0;
    
    // Circular orbit velocity: v = sqrt(GM/r)
    double v = sqrt(1.0 * bodies[0].mass / r);
    bodies[1].vx = 0.0;
    bodies[1].vy = v;
    bodies[1].mass = 1.0;
    
    // Simulate for one orbit period
    double period = 2.0 * M_PI * sqrt(r * r * r / (1.0 * bodies[0].mass));
    int steps = (int)(period / 0.001);  // Small timestep
    
    for (int i = 0; i < steps; i++) {
        update_bodies(bodies, 2, 0.001, 1.0);
    }
    
    // After one orbit, should be back near starting position
    double final_r = sqrt(bodies[1].x * bodies[1].x + bodies[1].y * bodies[1].y);
    int passed = fabs(final_r - r) < 0.1;  // Within 5% error
    
    test_result("Circular orbit stability", passed);
}

/**
 * Test 4: Energy conservation
 * Total energy should remain approximately constant
 */
void test_energy_conservation() {
    Body bodies[2];
    
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].vx = 0.0;
    bodies[0].vy = 0.0;
    bodies[0].mass = 100.0;
    
    bodies[1].x = 3.0;
    bodies[1].y = 0.0;
    bodies[1].vx = 0.0;
    bodies[1].vy = 5.0;
    bodies[1].mass = 1.0;
    
    // Calculate initial energy
    double ke0 = 0.5 * bodies[1].mass * (bodies[1].vx * bodies[1].vx + 
                                         bodies[1].vy * bodies[1].vy);
    double r0 = sqrt(bodies[1].x * bodies[1].x + bodies[1].y * bodies[1].y);
    double pe0 = -1.0 * bodies[0].mass * bodies[1].mass / r0;
    double E0 = ke0 + pe0;
    
    // Simulate for some time
    for (int i = 0; i < 1000; i++) {
        update_bodies(bodies, 2, 0.01, 1.0);
    }
    
    // Calculate final energy
    double ke1 = 0.5 * bodies[1].mass * (bodies[1].vx * bodies[1].vx + 
                                         bodies[1].vy * bodies[1].vy);
    double r1 = sqrt(bodies[1].x * bodies[1].x + bodies[1].y * bodies[1].y);
    double pe1 = -1.0 * bodies[0].mass * bodies[1].mass / r1;
    double E1 = ke1 + pe1;
    
    // Energy should be conserved within reasonable tolerance
    double energy_error = fabs((E1 - E0) / E0);
    int passed = energy_error < 0.01;  // Within 1% error
    
    test_result("Energy conservation", passed);
}

/**
 * Test 5: Rocket force calculation
 * Rockets should experience correct gravitational acceleration
 */
void test_rocket_forces() {
    Body bodies[1];
    Rocket rockets[1];
    
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].mass = 100.0;
    
    rockets[0].x = 3.0;
    rockets[0].y = 4.0;  // Distance = 5.0
    rockets[0].active = 1;
    
    compute_rocket_forces(rockets, 1, bodies, 1, 1.0);
    
    // Expected acceleration magnitude: a = GM/r^2 = 1.0 * 100 / 25 = 4.0
    double expected_mag = 4.0;
    double actual_mag = sqrt(rockets[0].ax * rockets[0].ax + 
                            rockets[0].ay * rockets[0].ay);
    
    int passed = fabs(actual_mag - expected_mag) < EPSILON;
    
    test_result("Rocket gravitational acceleration", passed);
}

/**
 * Test 6: Multi-body system
 * Test force calculations with 3+ bodies
 */
void test_multi_body() {
    Body bodies[3];
    
    // Create equilateral triangle configuration
    bodies[0].x = 0.0;
    bodies[0].y = 0.0;
    bodies[0].mass = 10.0;
    
    bodies[1].x = 1.0;
    bodies[1].y = 0.0;
    bodies[1].mass = 10.0;
    
    bodies[2].x = 0.5;
    bodies[2].y = sqrt(3.0) / 2.0;
    bodies[2].mass = 10.0;
    
    compute_forces(bodies, 3, 1.0);
    
    // By symmetry, net force on each body should have same magnitude
    double f0 = sqrt(bodies[0].ax * bodies[0].ax + bodies[0].ay * bodies[0].ay);
    double f1 = sqrt(bodies[1].ax * bodies[1].ax + bodies[1].ay * bodies[1].ay);
    double f2 = sqrt(bodies[2].ax * bodies[2].ax + bodies[2].ay * bodies[2].ay);
    
    int passed = (fabs(f0 - f1) < EPSILON) && (fabs(f1 - f2) < EPSILON);
    
    test_result("Multi-body force symmetry", passed);
}

int main() {
    printf("\n");
    printf("====================================\n");
    printf("Physics Module Test Suite\n");
    printf("====================================\n\n");
    
    test_two_body_force();
    test_force_symmetry();
    test_circular_orbit();
    test_energy_conservation();
    test_rocket_forces();
    test_multi_body();
    
    printf("\n====================================\n");
    printf("Results: %d/%d tests passed\n", tests_passed, tests_run);
    printf("====================================\n\n");
    
    return (tests_passed == tests_run) ? 0 : 1;
}