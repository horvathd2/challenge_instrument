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
CMAKE_SOURCE_DIR = C:\pico\challenge_instrument-1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\pico\challenge_instrument-1\build

# Utility rule file for ELF2UF2Build.

# Include any custom commands dependencies for this target.
include main/CMakeFiles/ELF2UF2Build.dir/compiler_depend.make

# Include the progress variables for this target.
include main/CMakeFiles/ELF2UF2Build.dir/progress.make

main/CMakeFiles/ELF2UF2Build: main/CMakeFiles/ELF2UF2Build-complete

main/CMakeFiles/ELF2UF2Build-complete: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-install
main/CMakeFiles/ELF2UF2Build-complete: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir
main/CMakeFiles/ELF2UF2Build-complete: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download
main/CMakeFiles/ELF2UF2Build-complete: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update
main/CMakeFiles/ELF2UF2Build-complete: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch
main/CMakeFiles/ELF2UF2Build-complete: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure
main/CMakeFiles/ELF2UF2Build-complete: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-build
main/CMakeFiles/ELF2UF2Build-complete: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument-1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'ELF2UF2Build'"
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E make_directory C:/pico/challenge_instrument-1/build/main/CMakeFiles
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/pico/challenge_instrument-1/build/main/CMakeFiles/ELF2UF2Build-complete
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/pico/challenge_instrument-1/build/main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-done

main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-build: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument-1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'ELF2UF2Build'"
	cd /d C:\pico\challenge_instrument-1\build\elf2uf2 && $(MAKE)

main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure: main/elf2uf2/tmp/ELF2UF2Build-cfgcmd.txt
main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument-1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'ELF2UF2Build'"
	cd /d C:\pico\challenge_instrument-1\build\elf2uf2 && "C:\Program Files\CMake\bin\cmake.exe" -DCMAKE_MAKE_PROGRAM:FILEPATH=C:/MinGW/bin/mingw32-make.exe "-GMinGW Makefiles" -S C:/pico/pico-sdk/tools/elf2uf2 -B C:/pico/challenge_instrument-1/build/elf2uf2
	cd /d C:\pico\challenge_instrument-1\build\elf2uf2 && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/pico/challenge_instrument-1/build/main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure

main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-source_dirinfo.txt
main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument-1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'ELF2UF2Build'"
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/pico/challenge_instrument-1/build/main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download

main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-install: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument-1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'ELF2UF2Build'"
	cd /d C:\pico\challenge_instrument-1\build\elf2uf2 && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append

main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument-1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'ELF2UF2Build'"
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -Dcfgdir= -P C:/pico/challenge_instrument-1/build/main/elf2uf2/tmp/ELF2UF2Build-mkdirs.cmake
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/pico/challenge_instrument-1/build/main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir

main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument-1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'ELF2UF2Build'"
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/pico/challenge_instrument-1/build/main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch

main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\pico\challenge_instrument-1\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No update step for 'ELF2UF2Build'"
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E echo_append
	cd /d C:\pico\challenge_instrument-1\build\main && "C:\Program Files\CMake\bin\cmake.exe" -E touch C:/pico/challenge_instrument-1/build/main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update

ELF2UF2Build: main/CMakeFiles/ELF2UF2Build
ELF2UF2Build: main/CMakeFiles/ELF2UF2Build-complete
ELF2UF2Build: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-build
ELF2UF2Build: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure
ELF2UF2Build: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download
ELF2UF2Build: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-install
ELF2UF2Build: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir
ELF2UF2Build: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch
ELF2UF2Build: main/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update
ELF2UF2Build: main/CMakeFiles/ELF2UF2Build.dir/build.make
.PHONY : ELF2UF2Build

# Rule to build all files generated by this target.
main/CMakeFiles/ELF2UF2Build.dir/build: ELF2UF2Build
.PHONY : main/CMakeFiles/ELF2UF2Build.dir/build

main/CMakeFiles/ELF2UF2Build.dir/clean:
	cd /d C:\pico\challenge_instrument-1\build\main && $(CMAKE_COMMAND) -P CMakeFiles\ELF2UF2Build.dir\cmake_clean.cmake
.PHONY : main/CMakeFiles/ELF2UF2Build.dir/clean

main/CMakeFiles/ELF2UF2Build.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\pico\challenge_instrument-1 C:\pico\challenge_instrument-1\main C:\pico\challenge_instrument-1\build C:\pico\challenge_instrument-1\build\main C:\pico\challenge_instrument-1\build\main\CMakeFiles\ELF2UF2Build.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : main/CMakeFiles/ELF2UF2Build.dir/depend

