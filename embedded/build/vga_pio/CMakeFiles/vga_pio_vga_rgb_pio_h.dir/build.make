# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\build

# Utility rule file for vga_pio_vga_rgb_pio_h.

# Include any custom commands dependencies for this target.
include vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/progress.make

vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h: vga_pio/vga_rgb.pio.h

vga_pio/vga_rgb.pio.h: D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/vga_pio/vga_rgb.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating vga_rgb.pio.h"
	cd /d D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\build\vga_pio && ..\pioasm\pioasm.exe -o c-sdk D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/vga_pio/vga_rgb.pio D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga_pio/vga_rgb.pio.h

vga_pio_vga_rgb_pio_h: vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h
vga_pio_vga_rgb_pio_h: vga_pio/vga_rgb.pio.h
vga_pio_vga_rgb_pio_h: vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/build.make
.PHONY : vga_pio_vga_rgb_pio_h

# Rule to build all files generated by this target.
vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/build: vga_pio_vga_rgb_pio_h
.PHONY : vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/build

vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/clean:
	cd /d D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\build\vga_pio && $(CMAKE_COMMAND) -P CMakeFiles\vga_pio_vga_rgb_pio_h.dir\cmake_clean.cmake
.PHONY : vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/clean

vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\vga_pio D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\build D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\build\vga_pio D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\build\vga_pio\CMakeFiles\vga_pio_vga_rgb_pio_h.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : vga_pio/CMakeFiles/vga_pio_vga_rgb_pio_h.dir/depend

