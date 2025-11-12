# Student Exercises - N-Body Simulation Extensions

This document contains hands-on exercises for extending the N-body rocket simulation. These exercises range from beginner to advanced and cover visualization, physics, and software engineering concepts.

---

## 🎨 Level 1: Visualization Enhancements

### Exercise 1.1: Color-Coded Velocities
**Difficulty**: ⭐ Easy  
**Goal**: Color rocket trails based on speed

**Task**: Modify `render.c` to color the rocket trail based on the rocket's velocity at each point. Faster = red, slower = blue.

**Hints**:
- Calculate speed: `speed = sqrt(vx*vx + vy*vy)`
- Map speed to RGB color (use min/max speed for normalization)
- Store velocities in rocket trail arrays

**Expected Outcome**: Trails that change color showing acceleration/deceleration

---

### Exercise 1.2: Body Trails
**Difficulty**: ⭐ Easy  
**Goal**: Show orbital paths for planets

**Task**: Add trail rendering for celestial bodies (not just rockets). Each body should leave a fading trail showing its orbit.

**Hints**:
- Add `trail_x` and `trail_y` arrays to `Body` structure
- Update `update_bodies()` to record positions
- Render trails in `render()` before drawing bodies

**Expected Outcome**: Beautiful spiral patterns for multi-body systems

---

### Exercise 1.3: Real-Time Display
**Difficulty**: ⭐⭐ Medium  
**Goal**: Replace BMP output with SDL2 real-time visualization

**Task**: Create a new rendering module using SDL2 that displays the simulation in real-time instead of generating BMP files.

**Hints**:
- Install SDL2: `sudo apt-get install libsdl2-dev`
- Create `render_sdl.c` with window initialization
- Update main loop to draw each frame to screen
- Add keyboard controls (space = pause, ESC = quit)

**Expected Outcome**: Interactive window showing simulation as it runs

---

### Exercise 1.4: Zoom and Pan
**Difficulty**: ⭐⭐ Medium  
**Goal**: Add camera controls

**Task**: Implement zoom and pan controls for the visualization. Allow user to focus on specific parts of the simulation.

**Hints**:
- Add camera position (cx, cy) and zoom level to rendering
- Transform world coordinates: `screen_x = (world_x - cx) * zoom + WIDTH/2`
- Bind mouse wheel to zoom, drag to pan

**Expected Outcome**: Ability to follow individual bodies or get overview

---

### Exercise 1.5: Data Overlay
**Difficulty**: ⭐⭐ Medium  
**Goal**: Display simulation statistics on screen

**Task**: Overlay text showing current step, total energy, number of active rockets, FPS, etc.

**Hints**:
- Use SDL_ttf for text rendering or draw simple pixel font
- Calculate total kinetic and potential energy
- Update display each frame

**Expected Outcome**: HUD showing simulation state

---

## 🔬 Level 2: Physics Extensions

### Exercise 2.1: Collision Detection
**Difficulty**: ⭐⭐ Medium  
**Goal**: Detect and handle collisions between bodies

**Task**: Implement collision detection. When two bodies collide, merge them or have them bounce.

**Hints**:
- Check distance between all body pairs each step
- Collision when: `distance < (radius1 + radius2)`
- For merge: create new body with combined mass and momentum
- For bounce: exchange velocity components

**Expected Outcome**: Bodies can collide and interact

**Physics**: Conservation of momentum: `m1*v1 + m2*v2 = (m1+m2)*v_final`

---

### Exercise 2.2: Variable Timestep
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Adaptive timestep for better accuracy

**Task**: Implement adaptive timestep that decreases when bodies are close together (where forces are stronger).

**Hints**:
- Calculate minimum distance between all bodies
- Adjust dt inversely: `dt_adaptive = dt_base * sqrt(min_distance)`
- Cap maximum timestep for stability

**Expected Outcome**: Better energy conservation, more accurate orbits

---

### Exercise 2.3: Atmospheric Drag
**Difficulty**: ⭐⭐ Medium  
**Goal**: Add drag force to rockets

**Task**: Implement atmospheric drag that slows down rockets near planets.

**Hints**:
- Drag force: `F_drag = -k * v * |v|` (proportional to velocity squared)
- Drag increases near bodies: `k = k0 * exp(-distance/scale_height)`
- Add drag acceleration to rocket updates

**Expected Outcome**: Rockets gradually lose energy and spiral inward

**Extensions**:
- Add thrust to rockets to counteract drag
- Implement orbital decay

---

### Exercise 2.4: Gravitational Slingshot Calculator
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Predict gravity assist trajectories

**Task**: Create a tool that calculates the optimal trajectory for a rocket to gain speed using a gravity assist maneuver.

**Hints**:
- Calculate approach angle and velocity for maximum energy gain
- Use conservation of energy in rotating reference frame
- Show predicted path before launching rocket

**Expected Outcome**: Tool for planning interplanetary missions

---

### Exercise 2.5: Tidal Forces
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Simulate tidal effects on extended bodies

**Task**: Instead of point masses, make bodies have radius. Show tidal deformation when bodies get close.

**Hints**:
- Model body as collection of point masses in rigid formation
- Calculate differential forces across body
- Visualize deformation with ellipse shape

**Expected Outcome**: Bodies stretch when near massive objects

---

## 🎮 Level 3: Interactive Features

### Exercise 3.1: Rocket Launcher
**Difficulty**: ⭐⭐ Medium  
**Goal**: Click-and-drag to launch rockets

**Task**: Allow user to click on screen to set rocket position, drag to set velocity vector, release to launch.

**Hints**:
- Capture mouse down position for rocket start
- Draw arrow while dragging showing velocity
- On mouse up, create rocket with calculated velocity
- Display velocity magnitude

**Expected Outcome**: Interactive rocket launching

---

### Exercise 3.2: Body Editor
**Difficulty**: ⭐⭐ Medium  
**Goal**: Add/remove/modify bodies during simulation

**Task**: Create UI for adding new bodies, adjusting their mass, or removing them from the simulation.

**Hints**:
- Add keyboard shortcuts (B = new body, D = delete, M = modify mass)
- Click to select body, show properties
- Allow dragging bodies to new positions

**Expected Outcome**: Full interactive simulation sandbox

---

### Exercise 3.3: Preset Scenarios
**Difficulty**: ⭐ Easy  
**Goal**: Pre-defined interesting configurations

**Task**: Create a menu of preset scenarios (binary star, figure-8 orbit, solar system, etc.)

**Hints**:
- Create initialization functions for each scenario
- Add number keys to load presets
- Include: binary star, lagrange points, chaotic 3-body

**Expected Outcome**: Quick access to interesting simulations

**Scenarios to implement**:
1. Binary star system
2. Figure-8 choreography (3 equal masses)
3. Lagrange points demonstration
4. Solar system (Sun + 8 planets)
5. Galaxy collision

---

### Exercise 3.4: Time Controls
**Difficulty**: ⭐ Easy  
**Goal**: Speed up, slow down, or reverse time

**Task**: Add controls to adjust simulation speed and allow time reversal.

**Hints**:
- Multiply dt by speed factor (0.5 = half speed, 2.0 = double speed)
- For reverse: negate dt
- Add keys: + (faster), - (slower), R (reverse)

**Expected Outcome**: Full time control like video player

---

### Exercise 3.5: Save/Load Scenarios
**Difficulty**: ⭐⭐ Medium  
**Goal**: Save current state and reload later

**Task**: Implement save/load functionality to save the current state of all bodies and rockets to a file.

**Hints**:
- Save to JSON or binary format
- Include positions, velocities, masses
- Add timestamp and metadata
- Add keys: S (save), L (load)

**Expected Outcome**: Ability to share interesting configurations

---

## 🚀 Level 4: Advanced Features

### Exercise 4.1: N-Body Barnes-Hut Algorithm
**Difficulty**: ⭐⭐⭐⭐ Very Hard  
**Goal**: Optimize force calculation from O(N²) to O(N log N)

**Task**: Implement the Barnes-Hut algorithm using a quadtree for faster simulation of many bodies.

**Hints**:
- Build quadtree dividing space into cells
- For distant cells, approximate as single body at center of mass
- Use threshold parameter θ for accuracy vs speed tradeoff
- Rebuild tree each timestep

**Expected Outcome**: Simulate 1000+ bodies in real-time

**Resources**:
- [Barnes-Hut Algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation)

---

### Exercise 4.2: Relativistic Effects
**Difficulty**: ⭐⭐⭐⭐ Very Hard  
**Goal**: Add special relativity corrections

**Task**: Implement relativistic corrections for very fast-moving objects.

**Hints**:
- Use relativistic momentum: `p = γ * m * v` where `γ = 1/sqrt(1 - v²/c²)`
- Add speed of light constant
- Only apply corrections when v > 0.1c
- Update force calculations with relativistic factors

**Expected Outcome**: Accurate simulation at high velocities

---

### Exercise 4.3: 3D Simulation
**Difficulty**: ⭐⭐⭐⭐ Very Hard  
**Goal**: Extend to 3 dimensions

**Task**: Convert entire simulation from 2D to 3D with proper 3D rendering.

**Hints**:
- Add z components to all vectors
- Use perspective projection for rendering
- Implement camera rotation (quaternions or Euler angles)
- Use OpenGL or similar for 3D graphics

**Expected Outcome**: Full 3D N-body simulation

---

### Exercise 4.4: Parallel Computing
**Difficulty**: ⭐⭐⭐⭐ Very Hard  
**Goal**: Use multiple CPU cores with OpenMP

**Task**: Parallelize force calculations using OpenMP to utilize multiple CPU cores.

**Hints**:
- Add `#pragma omp parallel for` to force calculation loops
- Compile with `-fopenmp` flag
- Use reduction for force accumulation
- Profile performance improvement

**Expected Outcome**: Near-linear speedup with number of cores

---

### Exercise 4.5: GPU Acceleration
**Difficulty**: ⭐⭐⭐⭐⭐ Expert  
**Goal**: Port force calculations to GPU using CUDA or OpenCL

**Task**: Implement force calculations on GPU for massive speedup.

**Hints**:
- Each thread calculates forces on one body
- Use shared memory for body positions
- Handle memory transfers carefully
- Keep rendering on CPU initially

**Expected Outcome**: Simulate 10,000+ bodies in real-time

---

## 📊 Level 5: Analysis and Visualization

### Exercise 5.1: Phase Space Plot
**Difficulty**: ⭐⭐ Medium  
**Goal**: Plot position vs velocity

**Task**: Create phase space diagrams showing position on x-axis and velocity on y-axis.

**Hints**:
- For each rocket, plot (x, vx) as point
- Update plot each frame showing trajectory through phase space
- Different color for each rocket

**Expected Outcome**: Beautiful patterns showing conservation laws

---

### Exercise 5.2: Energy Conservation Graph
**Difficulty**: ⭐⭐ Medium  
**Goal**: Plot total energy over time

**Task**: Track and plot the total system energy throughout the simulation to verify conservation.

**Hints**:
- Calculate KE + PE each step
- Store in array
- Use gnuplot or matplotlib (via pipe) to plot
- Show percentage change from initial energy

**Expected Outcome**: Verify numerical accuracy

---

### Exercise 5.3: Orbital Elements Calculator
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Calculate and display Keplerian orbital elements

**Task**: For each rocket, calculate and display its orbital elements (semi-major axis, eccentricity, inclination, etc.)

**Hints**:
- Calculate specific orbital energy: `E = v²/2 - GM/r`
- Calculate angular momentum: `L = r × v`
- Derive semi-major axis: `a = -GM/(2E)`
- Calculate eccentricity from energy and angular momentum

**Expected Outcome**: Real orbital parameters displayed

**Elements to calculate**:
- Semi-major axis (a)
- Eccentricity (e)
- Periapsis and apoapsis distances
- Orbital period
- Inclination (for 3D)

---

### Exercise 5.4: Poincaré Section
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Create Poincaré sections to study chaos

**Task**: Plot points where orbits cross a specific plane (e.g., y=0 with vy>0) to reveal chaotic behavior.

**Hints**:
- Detect plane crossings each step
- Record (x, vx) at crossing
- Plot accumulated points
- Try different initial conditions

**Expected Outcome**: Fractal patterns in chaotic regions

---

### Exercise 5.5: Lyapunov Exponent Calculator
**Difficulty**: ⭐⭐⭐⭐ Very Hard  
**Goal**: Measure chaos in the system

**Task**: Calculate the Lyapunov exponent to quantify how quickly nearby trajectories diverge.

**Hints**:
- Simulate two slightly different initial conditions
- Measure separation growth over time
- Lyapunov exponent: λ = lim(t→∞) log(d(t)/d(0))/t
- Positive λ indicates chaos

**Expected Outcome**: Quantitative measure of chaos

---

## 🎓 Level 6: Real-World Applications

### Exercise 6.1: Solar System Simulation
**Difficulty**: ⭐⭐ Medium  
**Goal**: Accurate solar system model

**Task**: Use real planetary data to simulate the solar system with correct masses, distances, and velocities.

**Hints**:
- Use NASA ephemeris data
- Scale appropriately (1 AU = scale units)
- Use actual gravitational constant
- Include Sun + 8 planets + Moon

**Expected Outcome**: Realistic planetary motion

**Data sources**:
- NASA JPL Horizons System
- Wikipedia planetary data

---

### Exercise 6.2: Mission Planning Tool
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Plan interplanetary trajectories

**Task**: Create a tool for planning spacecraft missions between planets using Hohmann transfers.

**Hints**:
- Calculate Hohmann transfer orbit
- Determine launch windows
- Calculate ΔV requirements
- Visualize transfer trajectory

**Expected Outcome**: Mission planning calculator

---

### Exercise 6.3: Galaxy Simulation
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Simulate spiral galaxy formation

**Task**: Create initial conditions for galaxy simulation and watch spiral arms form.

**Hints**:
- Start with rotating disk of particles
- Add central supermassive black hole
- Use 1000+ particles
- Implement Barnes-Hut for performance

**Expected Outcome**: Beautiful spiral galaxy patterns

---

### Exercise 6.4: Binary Star System Analysis
**Difficulty**: ⭐⭐ Medium  
**Goal**: Study binary star orbits

**Task**: Simulate binary star systems and calculate the barycenter, orbital period, and stability.

**Hints**:
- Create equal-mass binary
- Track barycenter (center of mass)
- Measure orbital period
- Try different mass ratios

**Expected Outcome**: Understanding of binary dynamics

---

### Exercise 6.5: Asteroid Defense Simulation
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Simulate deflecting asteroids

**Task**: Model an asteroid approaching Earth and calculate the required impulse to deflect it.

**Hints**:
- Create asteroid on collision course
- Calculate intercept trajectory
- Apply impulse (change velocity)
- Show new trajectory
- Calculate minimum deflection time

**Expected Outcome**: Planetary defense tool

---

## 🏆 Challenge Projects

### Challenge 1: Lagrange Point Finder
**Difficulty**: ⭐⭐⭐⭐ Very Hard  
**Goal**: Automatically find and visualize Lagrange points

**Task**: Implement algorithm to find all 5 Lagrange points in a two-body system.

**Expected Outcome**: Marked L1-L5 points with stability indicators

---

### Challenge 2: Restricted Three-Body Problem
**Difficulty**: ⭐⭐⭐⭐ Very Hard  
**Goal**: Study the circular restricted three-body problem

**Task**: Implement CRTBP where two bodies orbit each other and a massless third body moves in their gravitational field.

**Expected Outcome**: Classic CRTBP dynamics visualization

---

### Challenge 3: Habitability Zone Calculator
**Difficulty**: ⭐⭐⭐ Hard  
**Goal**: Calculate and display habitable zones

**Task**: For a given star system, calculate and visualize the habitable zone where liquid water can exist.

**Expected Outcome**: Green overlay showing habitable zone

---

### Challenge 4: Multi-GPU Supercomputer Simulation
**Difficulty**: ⭐⭐⭐⭐⭐ Expert  
**Goal**: Scale to multiple GPUs

**Task**: Use MPI + CUDA to simulate 1,000,000+ bodies across multiple GPUs.

**Expected Outcome**: Massive scale simulation

---

## 📚 Learning Resources

### Physics
- Classical Mechanics by Taylor
- Goldstein's Classical Mechanics
- NASA's Orbital Mechanics course

### Numerical Methods
- Numerical Recipes in C
- Press et al., "Numerical Recipes"
- Hairer et al., "Solving Ordinary Differential Equations"

### Computer Graphics
- SDL2 documentation
- OpenGL tutorials
- Real-Time Rendering by Akenine-Möller

### Algorithms
- Barnes-Hut algorithm papers
- Parallel programming guides
- GPU computing tutorials

---

## 💡 Tips for Success

1. **Start Simple**: Begin with visualization exercises before complex physics
2. **Test Incrementally**: Test each feature before moving to the next
3. **Use Version Control**: Commit working code before major changes
4. **Profile Performance**: Use profilers to find bottlenecks
5. **Validate Physics**: Compare results to known analytical solutions
6. **Document Code**: Write comments explaining your approach
7. **Share Results**: Post videos/images of interesting results
8. **Ask for Help**: Join online communities (r/Physics, r/CFD)

---

## 🎯 Project Ideas for Portfolio

These extended projects make great portfolio pieces:

1. **Interactive Solar System Explorer** (Exercises 3.1-3.5 + 6.1)
2. **Mission Planning Tool** (Exercise 6.2 + real NASA data)
3. **GPU-Accelerated Galaxy Simulator** (Exercise 6.3 + 4.5)
4. **Real-Time 3D N-Body Visualization** (Exercise 4.3 + 1.3)
5. **Chaos Theory Explorer** (Exercises 5.4, 5.5, + visualizations)

Each of these demonstrates:
- Strong programming skills
- Physics knowledge
- Software engineering practices
- Graphics/visualization abilities
- Performance optimization

---

## 🤝 Contributing Your Solutions

If you create interesting extensions:

1. Fork the repository
2. Create a branch for your feature
3. Document your changes
4. Add your solution to the `extensions/` directory
5. Submit a pull request

Good documentation should include:
- What the extension does
- How to compile/run it
- Example output or screenshots
- Any new dependencies
- Performance characteristics

---

**Remember**: The goal is to learn and have fun! Don't worry if some exercises seem too difficult - work on what interests you and come back to harder problems later.

Happy coding! 🚀