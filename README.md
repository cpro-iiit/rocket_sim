# N-Body Rocket Simulation

A modular C program for simulating gravitational N-body dynamics with rocket trajectory tracking in 2D. Features complete file I/O capabilities, statistical analysis, and BMP visualization.

## 📁 Project Structure

```
nbody-simulation/
├── Makefile                    # Build configuration
├── README.md                   # This file
│
├── include/                    # Header files
│   └── nbody.h                # Main header with all declarations
│
├── src/                        # Source files
│   ├── main.c                 # Main program entry point
│   ├── bmp_io.c               # BMP image file I/O
│   ├── file_io.c              # Configuration and data file I/O
│   ├── init.c                 # Default initialization functions
│   ├── physics.c              # Physics simulation and integration
│   └── render.c               # Rendering and visualization
│
├── tools/                      # Analysis and utility tools
│   ├── analyze_trails.c       # Trajectory analysis tool
│   └── plot_trails.c          # Trajectory plotting tool
│
├── test/                       # Test cases
│   ├── test_physics.c         # Physics module tests
│   ├── test_file_io.c         # File I/O tests
│   ├── test_integration.c     # Integration tests
│   └── test_data/             # Test data files
│       ├── test_bodies.txt
│       ├── test_rockets.txt
│       └── test_config.txt
│
├── config/                     # Configuration files
│   ├── config.txt             # Simulation configuration (optional)
│   ├── bodies.txt             # Body definitions (optional)
│   └── rockets.txt            # Rocket initial conditions (optional)
│
├── obj/                        # Object files (created by make)
├── bin/                        # Executables (created by make)
└── output/                     # Simulation outputs (created by program)
```

## 🚀 Quick Start

### Prerequisites
- GCC compiler
- Make utility
- Standard C library with math support

### Building

```bash
# Build all programs
make

# Or build and run immediately
make run
```

### Running the Simulation

```bash
# Interactive mode
./bin/nbody

# Then choose:
# [N] - Run new simulation
# [L] - Load saved data
```

## 📋 Module Descriptions

### Core Modules (src/)

- **main.c** - Program entry point:
  - User interaction and mode selection
  - Coordination of initialization, simulation, and output
  - Main simulation loop

- **physics.c** - Physics engine:
  - Gravitational force calculations (Newton's law)
  - Numerical integration (leapfrog method)
  - Body and rocket position/velocity updates

- **render.c** - Visualization:
  - Converts simulation state to pixel representation
  - Draws trajectories, bodies, and grid
  - Bresenham line and circle algorithms

- **bmp_io.c** - Image file handling:
  - BMP file format writing
  - 24-bit color support
  - Row padding for BMP compliance

- **file_io.c** - Data persistence:
  - Load/save configuration files
  - Text file parsing for bodies and rockets
  - Binary trajectory storage
  - CSV statistics export
  - Metadata generation

- **init.c** - Default initialization:
  - Creates default solar system configuration
  - Initializes elliptical orbit for rockets
  - Fallback when config files are missing

### Header Files (include/)

- **nbody.h** - Central header file containing:
  - Data structure definitions (Body, Rocket, Pixel, SimConfig)
  - Constants and configuration parameters
  - Function prototypes for all modules

### Analysis Tools (tools/)

- **analyze_trails.c** - Statistical analysis:
  - Reads binary trajectory files
  - Computes distance metrics
  - Calculates path lengths
  - Reports bounding boxes

- **plot_trails.c** - Trajectory visualization:
  - Loads saved trajectory data
  - Renders complete paths to BMP
  - Multi-rocket color coding
  - No simulation re-run needed

### Test Suite (test/)

- **test_physics.c** - Physics module tests:
  - Force calculation validation
  - Energy conservation checks
  - Integration accuracy tests

- **test_file_io.c** - File I/O tests:
  - Configuration parsing
  - Body/rocket loading
  - Binary file operations

- **test_integration.c** - Integration tests:
  - End-to-end simulation tests
  - Output validation
  - Known orbit verification

## 🛠️ Makefile Targets

| Command | Description |
|---------|-------------|
| `make` or `make all` | Build all executables |
| `make run` | Build and run simulation |
| `make test` | Run all test cases |
| `make analyze` | Run trajectory analysis |
| `make plot` | Generate trajectory plot |
| `make clean` | Remove build artifacts |
| `make clean-output` | Remove simulation outputs |
| `make clean-all` | Full clean (build + output) |
| `make rebuild` | Clean and rebuild |
| `make install` | Install to system (needs sudo) |
| `make uninstall` | Remove from system |
| `make help` | Show all targets |
| `make check` | Verify dependencies |

## 📝 Configuration Files (config/)

### config.txt
```
dt=0.01
steps=5000
frames=100
save_interval=50
g=1.0
```

### bodies.txt
```
# x    y    vx   vy   mass
0.0   0.0  0.0  0.0  100.0    # Central star
2.0   0.0  0.0  0.707  5.0    # Planet 1
-2.0  0.0  0.0 -0.707  5.0    # Planet 2
```

### rockets.txt
```
# x     y    vx      vy
2.0    0.0  0.0     1.732     # Elliptical orbit
-8.0  -6.0  1.44    1.08      # Hyperbolic trajectory
```

## 📊 Output Files (output/)

| File | Description |
|------|-------------|
| `frame_XXXX.bmp` | Animation frames |
| `final_rockets.txt` | Final rocket states |
| `rocket_trails.bin` | Binary trajectory data |
| `rocket_stats.csv` | Statistical summary |
| `frames.log` | Frame generation log |
| `metadata.txt` | Simulation parameters |
| `plotted_trails.bmp` | Post-simulation plot |

## 🧪 Testing

### Running Tests
```bash
# Run all tests
make test

# Run specific test
./bin/test_physics
./bin/test_file_io
./bin/test_integration
```

### Test Coverage
- ✅ Physics calculations (force, acceleration)
- ✅ File parsing and validation
- ✅ Circular orbit stability
- ✅ Energy conservation
- ✅ Binary I/O correctness
- ✅ End-to-end simulation

## 🎓 Educational Exercises

This project implements all exercises from the C programming curriculum:

### Part 1: Input Extensions
- ✅ Exercise 1.1: Load bodies from file
- ✅ Exercise 1.2: Load rockets from file

### Part 2: Output Extensions
- ✅ Exercise 2.1: Save final rocket data
- ✅ Exercise 2.2: Binary trajectory storage
- ✅ Exercise 2.3: Frame metadata logging

### Part 3: Analysis Extensions
- ✅ Exercise 3.1: Trajectory statistics (CSV)
- ✅ Exercise 3.2: Reload and plot trails

### Part 4: Challenge Exercises
- ✅ Exercise 4.1: User interaction modes
- ✅ Exercise 4.2: Simulation metadata
- ✅ Exercise 4.3: Configurable parameters

## 🔧 Customization

### Orbital Mechanics Formulas

**Circular orbit velocity at radius r:**
```
v = sqrt(G * M / r)
```

**Elliptical orbit at perihelion:**
```
v_p = sqrt(G * M * (1 + e) / (a * (1 - e)))
where:
  a = semi-major axis
  e = eccentricity
```

**Escape velocity:**
```
v_escape = sqrt(2 * G * M / r)
```

### Adding New Bodies

Edit `config/bodies.txt`:
```
x  y  vx  vy  mass
3.0  0.0  0.0  0.577  2.0
```

### Adding New Rockets

Edit `config/rockets.txt`:
```
x  y  vx  vy
-5.0  -5.0  1.0  1.5
```

## 🐛 Troubleshooting

**"Could not open config.txt"**
- Config files are optional
- Program will use defaults if files are missing

**"Memory allocation failed"**
- Reduce WIDTH/HEIGHT in include/nbody.h
- Reduce STEPS or MAX_ROCKETS

**Rockets disappear immediately**
- Check initial position (too far from center)
- Adjust velocity (escape velocity exceeded)
- Verify bodies.txt has central mass

**Compilation errors**
- Ensure all files are in correct directories
- Check GCC is installed: `gcc --version`
- Try: `make clean && make`

## 📚 Algorithm Details

### Numerical Integration
- **Method**: Leapfrog (symplectic integrator)
- **Advantage**: Better energy conservation than Euler
- **Time step**: Configurable via `dt` parameter

### Force Calculation
- **Algorithm**: Direct N-body (O(N²))
- **Softening**: Prevents singularities when bodies are close
- **Optimization**: Newton's third law (compute each pair once)

### Rendering
- **Line drawing**: Bresenham's algorithm
- **Trajectory**: Gradient coloring (old→dim, new→bright)
- **Format**: 24-bit BMP (widely compatible)

## 🤝 Contributing

To extend this project:

1. Add new module to `src/` (e.g., `collision.c`)
2. Declare functions in `include/nbody.h`
3. Add source to `MAIN_SOURCES` in Makefile
4. Add tests to `test/` directory
5. Implement and test

## 📄 License

This is educational software. Free to use and modify for learning purposes.

## 🎯 Future Enhancements

- [ ] Collision detection
- [ ] Variable timestep integration
- [ ] 3D simulation support
- [ ] OpenGL real-time visualization
- [ ] MPI parallel processing
- [ ] Barnes-Hut tree algorithm (O(N log N))
- [ ] Energy/momentum conservation tracking
- [ ] Additional test coverage

## 📞 Support

For issues or questions, check:
1. This README
2. Comments in source code
3. Makefile help: `make help`
4. Test cases in `test/` directory

---

**Version**: 2.0  
**Author**: Educational C Programming Project  
**Date**: 2025