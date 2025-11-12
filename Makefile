# ==============================================================================
# N-Body Rocket Simulation - Simple Makefile with Dependencies
# ==============================================================================
# This Makefile uses minimal variables and explicit dependency rules.
# It automatically recompiles only files that have changed.
#
# To build everything: make
# To clean:           make clean
# To run:             make run
# For help:           make help
# ==============================================================================

# ==============================================================================
# BASIC CONFIGURATION - Only essential variables
# ==============================================================================

# The C compiler to use
CC = gcc

# Compiler flags (all warnings, optimization, C99 standard, include directory)
CFLAGS = -Wall -Wextra -O2 -std=c99 -Iinclude

# Linker flags (link with math library)
LDFLAGS = -lm

# Main header file that most files depend on
HEADER = include/nbody.h

# ==============================================================================
# DEFAULT TARGET - Builds main simulation
# ==============================================================================

# This runs when you type "make" with no arguments
all: bin/nbody
	@echo ""
	@echo "=========================================="
	@echo "✓ Build Complete!"
	@echo "=========================================="
	@echo "Main executable: bin/nbody"
	@echo ""
	@echo "Run with: make run"
	@echo "For help: make help"
	@echo ""

# ==============================================================================
# MAIN SIMULATION EXECUTABLE
# ==============================================================================

# The main executable depends on all object files
# If any .o file changes, the executable will be rebuilt
bin/nbody: obj/main.o obj/bmp_io.o obj/file_io.o obj/init.o obj/physics.o obj/render.o
	@mkdir -p bin
	@echo "Linking bin/nbody..."
	$(CC) obj/main.o obj/bmp_io.o obj/file_io.o obj/init.o obj/physics.o obj/render.o -o bin/nbody $(LDFLAGS)
	@echo "✓ Created bin/nbody"

# ==============================================================================
# OBJECT FILES - Only recompile if source or header changes
# ==============================================================================

# Each object file depends on its source file and the main header
# If either changes, that specific .o file will be recompiled

obj/main.o: src/main.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling src/main.c..."
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/bmp_io.o: src/bmp_io.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling src/bmp_io.c..."
	$(CC) $(CFLAGS) -c src/bmp_io.c -o obj/bmp_io.o

obj/file_io.o: src/file_io.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling src/file_io.c..."
	$(CC) $(CFLAGS) -c src/file_io.c -o obj/file_io.o

obj/init.o: src/init.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling src/init.c..."
	$(CC) $(CFLAGS) -c src/init.c -o obj/init.o

obj/physics.o: src/physics.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling src/physics.c..."
	$(CC) $(CFLAGS) -c src/physics.c -o obj/physics.o

obj/render.o: src/render.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling src/render.c..."
	$(CC) $(CFLAGS) -c src/render.c -o obj/render.o

# ==============================================================================
# ANALYSIS TOOLS
# ==============================================================================

# Build the trajectory analysis tool
bin/analyze_trails: obj/analyze_trails.o
	@mkdir -p bin
	@echo "Linking bin/analyze_trails..."
	$(CC) obj/analyze_trails.o -o bin/analyze_trails $(LDFLAGS)
	@echo "✓ Created bin/analyze_trails"

obj/analyze_trails.o: tools/analyze_trails.c
	@mkdir -p obj
	@echo "Compiling tools/analyze_trails.c..."
	$(CC) $(CFLAGS) -c tools/analyze_trails.c -o obj/analyze_trails.o

# Build the plotting tool
bin/plot_trails: obj/plot_trails.o
	@mkdir -p bin
	@echo "Linking bin/plot_trails..."
	$(CC) obj/plot_trails.o -o bin/plot_trails $(LDFLAGS)
	@echo "✓ Created bin/plot_trails"

obj/plot_trails.o: tools/plot_trails.c
	@mkdir -p obj
	@echo "Compiling tools/plot_trails.c..."
	$(CC) $(CFLAGS) -c tools/plot_trails.c -o obj/plot_trails.o

# Build all tools
tools: bin/analyze_trails bin/plot_trails
	@echo ""
	@echo "✓ All tools built"
	@echo ""

# ==============================================================================
# TEST PROGRAMS
# ==============================================================================

# Build physics test
bin/test_physics: obj/test_physics.o obj/bmp_io.o obj/file_io.o obj/init.o obj/physics.o obj/render.o
	@mkdir -p bin
	@echo "Linking bin/test_physics..."
	$(CC) obj/test_physics.o obj/bmp_io.o obj/file_io.o obj/init.o obj/physics.o obj/render.o -o bin/test_physics $(LDFLAGS)
	@echo "✓ Created bin/test_physics"

obj/test_physics.o: test/test_physics.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling test/test_physics.c..."
	$(CC) $(CFLAGS) -c test/test_physics.c -o obj/test_physics.o

# Build file I/O test
bin/test_file_io: obj/test_file_io.o obj/bmp_io.o obj/file_io.o obj/init.o obj/physics.o obj/render.o
	@mkdir -p bin
	@echo "Linking bin/test_file_io..."
	$(CC) obj/test_file_io.o obj/bmp_io.o obj/file_io.o obj/init.o obj/physics.o obj/render.o -o bin/test_file_io $(LDFLAGS)
	@echo "✓ Created bin/test_file_io"

obj/test_file_io.o: test/test_file_io.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling test/test_file_io.c..."
	$(CC) $(CFLAGS) -c test/test_file_io.c -o obj/test_file_io.o

# Build integration test
bin/test_integration: obj/test_integration.o obj/bmp_io.o obj/file_io.o obj/init.o obj/physics.o obj/render.o
	@mkdir -p bin
	@echo "Linking bin/test_integration..."
	$(CC) obj/test_integration.o obj/bmp_io.o obj/file_io.o obj/init.o obj/physics.o obj/render.o -o bin/test_integration $(LDFLAGS)
	@echo "✓ Created bin/test_integration"

obj/test_integration.o: test/test_integration.c $(HEADER)
	@mkdir -p obj
	@echo "Compiling test/test_integration.c..."
	$(CC) $(CFLAGS) -c test/test_integration.c -o obj/test_integration.o

# Build all tests
build-tests: bin/test_physics bin/test_file_io bin/test_integration
	@echo ""
	@echo "✓ All tests built"
	@echo ""

# ==============================================================================
# RUN PROGRAMS
# ==============================================================================

# Run the main simulation
run: bin/nbody
	@echo ""
	@echo "=========================================="
	@echo "Running Simulation"
	@echo "=========================================="
	@echo ""
	./bin/nbody

# Run all tests
test: build-tests
	@echo ""
	@echo "=========================================="
	@echo "Running Test Suite"
	@echo "=========================================="
	@echo ""
	@echo "Running physics tests..."
	./bin/test_physics
	@echo ""
	@echo "Running file I/O tests..."
	./bin/test_file_io
	@echo ""
	@echo "Running integration tests..."
	./bin/test_integration
	@echo ""
	@echo "=========================================="
	@echo "All Tests Complete"
	@echo "=========================================="
	@echo ""

# Run trajectory analysis
analyze: bin/analyze_trails
	@echo ""
	@echo "=========================================="
	@echo "Analyzing Trajectories"
	@echo "=========================================="
	@echo ""
	./bin/analyze_trails rocket_trails.bin

# Generate trajectory plot
plot: bin/plot_trails
	@echo ""
	@echo "=========================================="
	@echo "Plotting Trajectories"
	@echo "=========================================="
	@echo ""
	@mkdir -p output
	./bin/plot_trails rocket_trails.bin output/plotted_trails.bmp
	@echo ""
	@echo "Plot saved to output/plotted_trails.bmp"
	@echo ""

# ==============================================================================
# VIDEO GENERATION
# ==============================================================================

# Generate video from BMP frames using ffmpeg
video:
	@echo ""
	@echo "=========================================="
	@echo "Generating Video"
	@echo "=========================================="
	@echo ""
	@bash scripts/make_video.sh

# ==============================================================================
# SETUP AND CONFIGURATION
# ==============================================================================

# Create sample configuration files
init-config:
	@echo ""
	@echo "Creating sample configuration files..."
	@mkdir -p config
	@echo "# Simulation Configuration" > config/config.txt
	@echo "dt=0.01" >> config/config.txt
	@echo "steps=5000" >> config/config.txt
	@echo "frames=100" >> config/config.txt
	@echo "save_interval=50" >> config/config.txt
	@echo "g=1.0" >> config/config.txt
	@echo "" >> config/config.txt
	@echo "# x    y    vx   vy   mass" > config/bodies.txt
	@echo "0.0   0.0  0.0  0.0  100.0" >> config/bodies.txt
	@echo "2.0   0.0  0.0  0.707  5.0" >> config/bodies.txt
	@echo "-2.0  0.0  0.0 -0.707  5.0" >> config/bodies.txt
	@echo "" >> config/bodies.txt
	@echo "# x     y    vx      vy" > config/rockets.txt
	@echo "2.0    0.0  0.0     1.732" >> config/rockets.txt
	@echo "" >> config/rockets.txt
	@echo ""
	@echo "✓ Sample configuration files created in config/"
	@echo "  - config/config.txt"
	@echo "  - config/bodies.txt"
	@echo "  - config/rockets.txt"
	@echo ""

# ==============================================================================
# CLEANUP
# ==============================================================================

# Remove compiled files
clean:
	@echo ""
	@echo "Cleaning build artifacts..."
	rm -rf obj
	rm -rf bin
	@echo "✓ Build artifacts cleaned"
	@echo ""

# Remove simulation output files
clean-output:
	@echo ""
	@echo "Cleaning simulation outputs..."
	rm -rf output
	rm -f frame_*.bmp
	rm -f final_rockets.txt
	rm -f rocket_trails.bin
	rm -f rocket_stats.csv
	rm -f frames.log
	rm -f metadata.txt
	rm -f plotted_trails.bmp
	rm -f simulation.mp4
	@echo "✓ Output files cleaned"
	@echo ""

# Remove everything (build files and outputs)
clean-all: clean clean-output
	@echo "✓ Full clean complete"
	@echo ""

# ==============================================================================
# INSTALLATION (Optional - requires sudo)
# ==============================================================================

# Install to /usr/local/bin
install: bin/nbody bin/analyze_trails bin/plot_trails
	@echo ""
	@echo "Installing to /usr/local/bin..."
	@echo "(This requires sudo privileges)"
	@echo ""
	sudo install -m 755 bin/nbody /usr/local/bin/
	sudo install -m 755 bin/analyze_trails /usr/local/bin/
	sudo install -m 755 bin/plot_trails /usr/local/bin/
	@echo "✓ Installation complete"
	@echo ""

# Uninstall from system
uninstall:
	@echo ""
	@echo "Uninstalling from /usr/local/bin..."
	sudo rm -f /usr/local/bin/nbody
	sudo rm -f /usr/local/bin/analyze_trails
	sudo rm -f /usr/local/bin/plot_trails
	@echo "✓ Uninstall complete"
	@echo ""

# ==============================================================================
# UTILITY TARGETS
# ==============================================================================

# Rebuild everything from scratch
rebuild: clean all

# Check if required tools are available
check:
	@echo ""
	@echo "Checking dependencies..."
	@which $(CC) > /dev/null && echo "  [OK] $(CC) found" || echo "  [ERROR] $(CC) not found"
	@which ffmpeg > /dev/null && echo "  [OK] ffmpeg found" || echo "  [WARN] ffmpeg not found (optional)"
	@test -d include && echo "  [OK] include/ exists" || echo "  [ERROR] include/ missing"
	@test -d src && echo "  [OK] src/ exists" || echo "  [ERROR] src/ missing"
	@test -d tools && echo "  [OK] tools/ exists" || echo "  [ERROR] tools/ missing"
	@test -d test && echo "  [OK] test/ exists" || echo "  [ERROR] test/ missing"
	@echo ""

# Show project statistics
stats:
	@echo ""
	@echo "Project Statistics:"
	@echo "  Source files:    6"
	@echo "  Tool files:      2"
	@echo "  Test files:      3"
	@echo "  Header files:    1"
	@echo ""
	@echo "Total lines of code:"
	@find src tools test include -name '*.c' -o -name '*.h' 2>/dev/null | xargs wc -l 2>/dev/null | tail -1 || echo "  Run 'find' command to count"
	@echo ""

# ==============================================================================
# HELP
# ==============================================================================

# Show available targets and usage
help:
	@echo ""
	@echo "=========================================="
	@echo "N-Body Simulation Makefile"
	@echo "=========================================="
	@echo ""
	@echo "BASIC USAGE:"
	@echo "  make              Build main simulation"
	@echo "  make run          Build and run simulation"
	@echo "  make clean        Remove build files"
	@echo "  make help         Show this help message"
	@echo ""
	@echo "BUILD TARGETS:"
	@echo "  make all          Build main simulation (default)"
	@echo "  make tools        Build analysis tools"
	@echo "  make build-tests  Build test suite"
	@echo "  make rebuild      Clean and rebuild everything"
	@echo ""
	@echo "RUN TARGETS:"
	@echo "  make run          Run the simulation"
	@echo "  make test         Run all tests"
	@echo "  make analyze      Analyze trajectory data"
	@echo "  make plot         Generate trajectory plot"
	@echo "  make video        Create video from frames"
	@echo ""
	@echo "SETUP:"
	@echo "  make init-config  Create sample config files"
	@echo "  make install      Install to system (requires sudo)"
	@echo "  make uninstall    Remove from system"
	@echo ""
	@echo "CLEANUP:"
	@echo "  make clean        Remove build files"
	@echo "  make clean-output Remove simulation outputs"
	@echo "  make clean-all    Remove everything"
	@echo ""
	@echo "UTILITIES:"
	@echo "  make check        Check dependencies"
	@echo "  make stats        Show project statistics"
	@echo ""
	@echo "EXAMPLES:"
	@echo "  make && make run          Build and run"
	@echo "  make test                 Run test suite"
	@echo "  make video                Create animation"
	@echo "  make init-config && make  Setup and build"
	@echo ""
	@echo "=========================================="
	@echo ""

# ==============================================================================
# PHONY TARGETS - These are not actual files
# ==============================================================================

.PHONY: all run test analyze plot video clean clean-output clean-all
.PHONY: tools build-tests rebuild check stats help init-config install uninstall

# ==============================================================================
# HOW DEPENDENCIES WORK
# ==============================================================================
#
# Each rule has this format:
#   target: dependencies
#       commands
#
# Example:
#   bin/nbody: obj/main.o obj/bmp_io.o
#       gcc obj/main.o obj/bmp_io.o -o bin/nbody
#
# This means:
#   - bin/nbody depends on obj/main.o and obj/bmp_io.o
#   - If any .o file is newer than bin/nbody, rebuild bin/nbody
#   - If any .o file doesn't exist, build it first
#
# Example:
#   obj/main.o: src/main.c include/nbody.h
#       gcc -c src/main.c -o obj/main.o
#
# This means:
#   - obj/main.o depends on src/main.c and include/nbody.h
#   - If either source file is newer than obj/main.o, recompile
#   - This ensures changes to the header file trigger recompilation
#
# BENEFITS:
#   - Only changed files are recompiled (saves time)
#   - Dependencies are tracked automatically
#   - Always produces up-to-date executable
#
# TRY IT:
#   1. Run: make          (builds everything)
#   2. Run: make          (nothing happens - already up to date)
#   3. Edit: src/main.c
#   4. Run: make          (only main.c is recompiled and relinked)
#   5. Edit: include/nbody.h
#   6. Run: make          (all .c files are recompiled - they depend on .h)
#
# ===========================================================================