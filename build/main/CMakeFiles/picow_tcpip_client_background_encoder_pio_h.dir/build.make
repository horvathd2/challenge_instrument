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
CMAKE_SOURCE_DIR = C:\pico\challenge_instrument

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\pico\challenge_instrument\build

# Utility rule file for picow_tcpip_client_background_encoder_pio_h.

# Include any custom commands dependencies for this target.
include main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/progress.make

main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h: main/encoder.pio.h

main/encoder.pio.h: C:/pico/challenge_instrument/main/encoder.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating encoder.pio.h"
	cd /d C:\pico\challenge_instrument\build\main && ..\pioasm\pioasm.exe -o c-sdk C:/pico/challenge_instrument/main/encoder.pio C:/pico/challenge_instrument/build/main/encoder.pio.h

picow_tcpip_client_background_encoder_pio_h: main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h
picow_tcpip_client_background_encoder_pio_h: main/encoder.pio.h
picow_tcpip_client_background_encoder_pio_h: main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/build.make
.PHONY : picow_tcpip_client_background_encoder_pio_h

# Rule to build all files generated by this target.
main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/build: picow_tcpip_client_background_encoder_pio_h
.PHONY : main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/build

main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/clean:
	cd /d C:\pico\challenge_instrument\build\main && $(CMAKE_COMMAND) -P CMakeFiles\picow_tcpip_client_background_encoder_pio_h.dir\cmake_clean.cmake
.PHONY : main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/clean

main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\pico\challenge_instrument C:\pico\challenge_instrument\main C:\pico\challenge_instrument\build C:\pico\challenge_instrument\build\main C:\pico\challenge_instrument\build\main\CMakeFiles\picow_tcpip_client_background_encoder_pio_h.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : main/CMakeFiles/picow_tcpip_client_background_encoder_pio_h.dir/depend

