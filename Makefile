# Makefile for N-Body Rocket Simulation
# Professional modular build system with organized directory structure

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -Iinclude
LDFLAGS = -lm

# Directories
SRC_DIR = src
TOOLS_DIR = tools
TEST_DIR = test
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
CONFIG_DIR = config
OUTPUT_DIR = output

# Source files
MAIN_SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/bmp_io.c $(SRC_DIR)/file_io.c \
               $(SRC_DIR)/init.c $(SRC_DIR)/physics.c $(SRC_DIR)/render.c
MAIN_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(MAIN_SOURCES))
MAIN_TARGET = $(BIN_DIR)/nbody

# Analysis tool
ANALYZE_SOURCE = $(TOOLS_DIR)/analyze_trails.c
ANALYZE_OBJECT = $(OBJ_DIR)/analyze_trails.o
ANALYZE_TARGET = $(BIN_DIR)/analyze_trails

# Plotting tool
PLOT_SOURCE = $(TOOLS_DIR)/plot_trails.c
PLOT_OBJECT = $(OBJ_DIR)/plot_trails.o
PLOT_TARGET = $(BIN_DIR)/plot_trails

# Test files
TEST_PHYSICS_SOURCE = $(TEST_DIR)/test_physics.c
TEST_PHYSICS_OBJECT = $(OBJ_DIR)/test_physics.o
TEST_PHYSICS_TARGET = $(BIN_DIR)/test_physics

TEST_FILE_IO_SOURCE = $(TEST_DIR)/test_file_io.c
TEST_FILE_IO_OBJECT = $(OBJ_DIR)/test_file_io.o
TEST_FILE_IO_TARGET = $(BIN_DIR)/test_file_io

TEST_INTEGRATION_SOURCE = $(TEST_DIR)/test_integration.c
TEST_INTEGRATION_OBJECT = $(OBJ_DIR)/test_integration.o
TEST_INTEGRATION_TARGET = $(BIN_DIR)/test_integration

# All test targets
TEST_TARGETS = $(TEST_PHYSICS_TARGET) $(TEST_FILE_IO_TARGET) $(TEST_INTEGRATION_TARGET)

# Test objects that need main simulation objects (excluding main.c)
LIB_OBJECTS = $(filter-out $(OBJ_DIR)/main.o,$(MAIN_OBJECTS))

# Header files
HEADERS = $(INCLUDE_DIR)/nbody.h

# Colors for pretty output
COLOR_RESET = \033[0m
COLOR_BOLD = \033[1m
COLOR_GREEN = \033[32m
COLOR_BLUE = \033[34m
COLOR_YELLOW = \033[33m
COLOR_CYAN = \033[36m
COLOR_MAGENTA = \033[35m

# Default target: build everything
.PHONY: all
all: directories $(MAIN_TARGET) $(ANALYZE_TARGET) $(PLOT_TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ Build complete!$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)Executables:$(COLOR_RESET)"
	@echo "  - $(MAIN_TARGET)"
	@echo "  - $(ANALYZE_TARGET)"
	@echo "  - $(PLOT_TARGET)"
	@echo ""
	@echo "$(COLOR_CYAN)Run 'make help' for usage information$(COLOR_RESET)"

# Create necessary directories
.PHONY: directories
directories: $(OBJ_DIR) $(BIN_DIR) $(CONFIG_DIR) $(OUTPUT_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(COLOR_YELLOW)Created $(OBJ_DIR) directory$(COLOR_RESET)"

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)
	@echo "$(COLOR_YELLOW)Created $(BIN_DIR) directory$(COLOR_RESET)"

$(CONFIG_DIR):
	@mkdir -p $(CONFIG_DIR)
	@echo "$(COLOR_YELLOW)Created $(CONFIG_DIR) directory$(COLOR_RESET)"

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)
	@echo "$(COLOR_YELLOW)Created $(OUTPUT_DIR) directory$(COLOR_RESET)"

# Main simulation executable
$(MAIN_TARGET): $(MAIN_OBJECTS)
	@echo "$(COLOR_BLUE)Linking $@...$(COLOR_RESET)"
	@$(CC) $(MAIN_OBJECTS) -o $@ $(LDFLAGS)
	@echo "$(COLOR_GREEN)✓ Built $@$(COLOR_RESET)"

# Analysis tool executable
$(ANALYZE_TARGET): $(ANALYZE_OBJECT)
	@echo "$(COLOR_BLUE)Linking $@...$(COLOR_RESET)"
	@$(CC) $(ANALYZE_OBJECT) -o $@ $(LDFLAGS)
	@echo "$(COLOR_GREEN)✓ Built $@$(COLOR_RESET)"

# Plotting tool executable
$(PLOT_TARGET): $(PLOT_OBJECT)
	@echo "$(COLOR_BLUE)Linking $@...$(COLOR_RESET)"
	@$(CC) $(PLOT_OBJECT) -o $@ $(LDFLAGS)
	@echo "$(COLOR_GREEN)✓ Built $@$(COLOR_RESET)"

# Test executables
$(TEST_PHYSICS_TARGET): $(TEST_PHYSICS_OBJECT) $(LIB_OBJECTS)
	@echo "$(COLOR_BLUE)Linking $@...$(COLOR_RESET)"
	@$(CC) $(TEST_PHYSICS_OBJECT) $(LIB_OBJECTS) -o $@ $(LDFLAGS)
	@echo "$(COLOR_GREEN)✓ Built $@$(COLOR_RESET)"

$(TEST_FILE_IO_TARGET): $(TEST_FILE_IO_OBJECT) $(LIB_OBJECTS)
	@echo "$(COLOR_BLUE)Linking $@...$(COLOR_RESET)"
	@$(CC) $(TEST_FILE_IO_OBJECT) $(LIB_OBJECTS) -o $@ $(LDFLAGS)
	@echo "$(COLOR_GREEN)✓ Built $@$(COLOR_RESET)"

$(TEST_INTEGRATION_TARGET): $(TEST_INTEGRATION_OBJECT) $(LIB_OBJECTS)
	@echo "$(COLOR_BLUE)Linking $@...$(COLOR_RESET)"
	@$(CC) $(TEST_INTEGRATION_OBJECT) $(LIB_OBJECTS) -o $@ $(LDFLAGS)
	@echo "$(COLOR_GREEN)✓ Built $@$(COLOR_RESET)"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@echo "$(COLOR_BLUE)Compiling $<...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile tool files
$(OBJ_DIR)/%.o: $(TOOLS_DIR)/%.c $(HEADERS)
	@echo "$(COLOR_BLUE)Compiling $<...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile test files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c $(HEADERS)
	@echo "$(COLOR_BLUE)Compiling $<...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Build all tests
.PHONY: build-tests
build-tests: directories $(TEST_TARGETS)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ All tests built!$(COLOR_RESET)"

# Run all tests
.PHONY: test
test: build-tests
	@echo ""
	@echo "$(COLOR_MAGENTA)$(COLOR_BOLD)========================================$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)$(COLOR_BOLD)Running Test Suite$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)$(COLOR_BOLD)========================================$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_CYAN)Running Physics Tests...$(COLOR_RESET)"
	@$(TEST_PHYSICS_TARGET)
	@echo ""
	@echo "$(COLOR_CYAN)Running File I/O Tests...$(COLOR_RESET)"
	@$(TEST_FILE_IO_TARGET)
	@echo ""
	@echo "$(COLOR_CYAN)Running Integration Tests...$(COLOR_RESET)"
	@$(TEST_INTEGRATION_TARGET)
	@echo ""
	@echo "$(COLOR_MAGENTA)$(COLOR_BOLD)========================================$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)✓ All tests passed!$(COLOR_RESET)"
	@echo "$(COLOR_MAGENTA)$(COLOR_BOLD)========================================$(COLOR_RESET)"
	@echo ""

# Clean build artifacts
.PHONY: clean
clean:
	@echo "$(COLOR_YELLOW)Cleaning build artifacts...$(COLOR_RESET)"
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "$(COLOR_GREEN)✓ Clean complete$(COLOR_RESET)"

# Clean output files from simulation
.PHONY: clean-output
clean-output:
	@echo "$(COLOR_YELLOW)Cleaning simulation output files...$(COLOR_RESET)"
	@rm -rf $(OUTPUT_DIR)
	@rm -f frame_*.bmp
	@rm -f final_rockets.txt rocket_trails.bin rocket_stats.csv
	@rm -f frames.log metadata.txt plotted_trails.bmp
	@echo "$(COLOR_GREEN)✓ Output files cleaned$(COLOR_RESET)"

# Full clean (build + output)
.PHONY: clean-all
clean-all: clean clean-output
	@echo "$(COLOR_GREEN)✓ Full clean complete$(COLOR_RESET)"

# Run the main simulation
.PHONY: run
run: $(MAIN_TARGET) directories
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Running simulation...$(COLOR_RESET)"
	@cd $(BIN_DIR) && ./nbody

# Run analysis on saved data
.PHONY: analyze
analyze: $(ANALYZE_TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Analyzing trajectories...$(COLOR_RESET)"
	@$(ANALYZE_TARGET) rocket_trails.bin

# Plot saved trajectories
.PHONY: plot
plot: $(PLOT_TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Plotting trajectories...$(COLOR_RESET)"
	@$(PLOT_TARGET) rocket_trails.bin $(OUTPUT_DIR)/plotted_trails.bmp

# Create sample configuration files
.PHONY: init-config
init-config: directories
	@echo "$(COLOR_CYAN)Creating sample configuration files...$(COLOR_RESET)"
	@echo "# Simulation Configuration" > $(CONFIG_DIR)/config.txt
	@echo "dt=0.01" >> $(CONFIG_DIR)/config.txt
	@echo "steps=5000" >> $(CONFIG_DIR)/config.txt
	@echo "frames=100" >> $(CONFIG_DIR)/config.txt
	@echo "save_interval=50" >> $(CONFIG_DIR)/config.txt
	@echo "g=1.0" >> $(CONFIG_DIR)/config.txt
	@echo "# x    y    vx   vy   mass" > $(CONFIG_DIR)/bodies.txt
	@echo "0.0   0.0  0.0  0.0  100.0" >> $(CONFIG_DIR)/bodies.txt
	@echo "2.0   0.0  0.0  0.707  5.0" >> $(CONFIG_DIR)/bodies.txt
	@echo "-2.0  0.0  0.0 -0.707  5.0" >> $(CONFIG_DIR)/bodies.txt
	@echo "# x     y    vx      vy" > $(CONFIG_DIR)/rockets.txt
	@echo "2.0    0.0  0.0     1.732" >> $(CONFIG_DIR)/rockets.txt
	@echo "$(COLOR_GREEN)✓ Sample configuration files created in $(CONFIG_DIR)/$(COLOR_RESET)"

# Install executables to system (optional)
PREFIX ?= /usr/local
.PHONY: install
install: all
	@echo "$(COLOR_BLUE)Installing to $(PREFIX)/bin...$(COLOR_RESET)"
	@install -m 755 $(MAIN_TARGET) $(PREFIX)/bin/
	@install -m 755 $(ANALYZE_TARGET) $(PREFIX)/bin/
	@install -m 755 $(PLOT_TARGET) $(PREFIX)/bin/
	@echo "$(COLOR_GREEN)✓ Installation complete$(COLOR_RESET)"

# Uninstall from system
.PHONY: uninstall
uninstall:
	@echo "$(COLOR_YELLOW)Uninstalling from $(PREFIX)/bin...$(COLOR_RESET)"
	@rm -f $(PREFIX)/bin/nbody
	@rm -f $(PREFIX)/bin/analyze_trails
	@rm -f $(PREFIX)/bin/plot_trails
	@echo "$(COLOR_GREEN)✓ Uninstall complete$(COLOR_RESET)"

# Help target
.PHONY: help
help:
	@echo "$(COLOR_BOLD)N-Body Simulation Makefile$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BOLD)Build Targets:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)make$(COLOR_RESET) or $(COLOR_GREEN)make all$(COLOR_RESET)      - Build all executables"
	@echo "  $(COLOR_GREEN)make build-tests$(COLOR_RESET)        - Build all test cases"
	@echo "  $(COLOR_GREEN)make clean$(COLOR_RESET)              - Remove build artifacts"
	@echo "  $(COLOR_GREEN)make clean-output$(COLOR_RESET)       - Remove simulation outputs"
	@echo "  $(COLOR_GREEN)make clean-all$(COLOR_RESET)          - Remove everything"
	@echo "  $(COLOR_GREEN)make rebuild$(COLOR_RESET)            - Clean and rebuild"
	@echo ""
	@echo "$(COLOR_BOLD)Execution Targets:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)make run$(COLOR_RESET)                - Build and run simulation"
	@echo "  $(COLOR_GREEN)make test$(COLOR_RESET)               - Run all test cases"
	@echo "  $(COLOR_GREEN)make analyze$(COLOR_RESET)            - Run trajectory analysis"
	@echo "  $(COLOR_GREEN)make plot$(COLOR_RESET)               - Plot saved trajectories"
	@echo ""
	@echo "$(COLOR_BOLD)Setup Targets:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)make init-config$(COLOR_RESET)        - Create sample config files"
	@echo "  $(COLOR_GREEN)make install$(COLOR_RESET)            - Install to system (requires sudo)"
	@echo "  $(COLOR_GREEN)make uninstall$(COLOR_RESET)          - Remove from system"
	@echo ""
	@echo "$(COLOR_BOLD)Utility Targets:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)make help$(COLOR_RESET)               - Show this help message"
	@echo "  $(COLOR_GREEN)make check$(COLOR_RESET)              - Verify dependencies"
	@echo "  $(COLOR_GREEN)make debug$(COLOR_RESET)              - Show makefile variables"
	@echo ""
	@echo "$(COLOR_BOLD)Examples:$(COLOR_RESET)"
	@echo "  make init-config  # Create sample configuration files"
	@echo "  make              # Build everything"
	@echo "  make test         # Run test suite"
	@echo "  make run          # Build and run simulation"
	@echo "  make analyze      # Analyze results"
	@echo "  make plot         # Visualize trajectories"
	@echo "  make clean-all    # Clean everything"
	@echo ""
	@echo "$(COLOR_BOLD)Directory Structure:$(COLOR_RESET)"
	@echo "  $(SRC_DIR)/           - Source files"
	@echo "  $(TOOLS_DIR)/         - Analysis tools"
	@echo "  $(TEST_DIR)/          - Test cases"
	@echo "  $(INCLUDE_DIR)/       - Header files"
	@echo "  $(CONFIG_DIR)/        - Configuration files"
	@echo "  $(OBJ_DIR)/           - Object files (generated)"
	@echo "  $(BIN_DIR)/           - Executables (generated)"
	@echo "  $(OUTPUT_DIR)/        - Simulation outputs (generated)"

# Debug target to show variables
.PHONY: debug
debug:
	@echo "$(COLOR_BOLD)Makefile Variables:$(COLOR_RESET)"
	@echo "CC = $(CC)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo ""
	@echo "$(COLOR_BOLD)Directories:$(COLOR_RESET)"
	@echo "SRC_DIR = $(SRC_DIR)"
	@echo "TOOLS_DIR = $(TOOLS_DIR)"
	@echo "TEST_DIR = $(TEST_DIR)"
	@echo "INCLUDE_DIR = $(INCLUDE_DIR)"
	@echo "OBJ_DIR = $(OBJ_DIR)"
	@echo "BIN_DIR = $(BIN_DIR)"
	@echo "CONFIG_DIR = $(CONFIG_DIR)"
	@echo "OUTPUT_DIR = $(OUTPUT_DIR)"
	@echo ""
	@echo "$(COLOR_BOLD)Sources:$(COLOR_RESET)"
	@echo "MAIN_SOURCES = $(MAIN_SOURCES)"
	@echo "MAIN_OBJECTS = $(MAIN_OBJECTS)"
	@echo "LIB_OBJECTS = $(LIB_OBJECTS)"
	@echo ""
	@echo "$(COLOR_BOLD)Targets:$(COLOR_RESET)"
	@echo "MAIN_TARGET = $(MAIN_TARGET)"
	@echo "ANALYZE_TARGET = $(ANALYZE_TARGET)"
	@echo "PLOT_TARGET = $(PLOT_TARGET)"
	@echo "TEST_TARGETS = $(TEST_TARGETS)"

# Rebuild everything from scratch
.PHONY: rebuild
rebuild: clean all

# Check dependencies
.PHONY: check
check:
	@echo "$(COLOR_BLUE)Checking dependencies...$(COLOR_RESET)"
	@which $(CC) > /dev/null && echo "$(COLOR_GREEN)✓ $(CC) found$(COLOR_RESET)" || echo "$(COLOR_RED)✗ $(CC) not found$(COLOR_RESET)"
	@test -d $(INCLUDE_DIR) && echo "$(COLOR_GREEN)✓ $(INCLUDE_DIR)/ exists$(COLOR_RESET)" || echo "$(COLOR_YELLOW)⚠ $(INCLUDE_DIR)/ missing$(COLOR_RESET)"
	@test -d $(SRC_DIR) && echo "$(COLOR_GREEN)✓ $(SRC_DIR)/ exists$(COLOR_RESET)" || echo "$(COLOR_YELLOW)⚠ $(SRC_DIR)/ missing$(COLOR_RESET)"
	@test -d $(TOOLS_DIR) && echo "$(COLOR_GREEN)✓ $(TOOLS_DIR)/ exists$(COLOR_RESET)" || echo "$(COLOR_YELLOW)⚠ $(TOOLS_DIR)/ missing$(COLOR_RESET)"
	@test -d $(TEST_DIR) && echo "$(COLOR_GREEN)✓ $(TEST_DIR)/ exists$(COLOR_RESET)" || echo "$(COLOR_YELLOW)⚠ $(TEST_DIR)/ missing$(COLOR_RESET)"
	@echo "$(COLOR_GREEN)✓ Dependency check complete$(COLOR_RESET)"

# Show project statistics
.PHONY: stats
stats:
	@echo "$(COLOR_BOLD)Project Statistics:$(COLOR_RESET)"
	@echo "Source files: $$(find $(SRC_DIR) -name '*.c' | wc -l)"
	@echo "Header files: $$(find $(INCLUDE_DIR) -name '*.h' | wc -l)"
	@echo "Tool files: $$(find $(TOOLS_DIR) -name '*.c' | wc -l)"
	@echo "Test files: $$(find $(TEST_DIR) -name '*.c' | wc -l)"
	@echo ""
	@echo "Lines of code:"
	@wc -l $(SRC_DIR)/*.c $(TOOLS_DIR)/*.c $(TEST_DIR)/*.c $(INCLUDE_DIR)/*.h 2>/dev/null | tail -1 || echo "N/A"