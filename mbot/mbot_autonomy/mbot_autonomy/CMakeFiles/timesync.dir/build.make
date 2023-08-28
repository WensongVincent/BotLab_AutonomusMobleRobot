# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/botlab-w23/mbot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/botlab-w23/mbot/mbot_autonomy

# Include any dependencies generated for this target.
include mbot_autonomy/CMakeFiles/timesync.dir/depend.make

# Include the progress variables for this target.
include mbot_autonomy/CMakeFiles/timesync.dir/progress.make

# Include the compile flags for this target's objects.
include mbot_autonomy/CMakeFiles/timesync.dir/flags.make

mbot_autonomy/CMakeFiles/timesync.dir/src/mbot/timesync.cpp.o: mbot_autonomy/CMakeFiles/timesync.dir/flags.make
mbot_autonomy/CMakeFiles/timesync.dir/src/mbot/timesync.cpp.o: src/mbot/timesync.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/botlab-w23/mbot/mbot_autonomy/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object mbot_autonomy/CMakeFiles/timesync.dir/src/mbot/timesync.cpp.o"
	cd /home/pi/botlab-w23/mbot/mbot_autonomy/mbot_autonomy && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/timesync.dir/src/mbot/timesync.cpp.o -c /home/pi/botlab-w23/mbot/mbot_autonomy/src/mbot/timesync.cpp

mbot_autonomy/CMakeFiles/timesync.dir/src/mbot/timesync.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/timesync.dir/src/mbot/timesync.cpp.i"
	cd /home/pi/botlab-w23/mbot/mbot_autonomy/mbot_autonomy && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/botlab-w23/mbot/mbot_autonomy/src/mbot/timesync.cpp > CMakeFiles/timesync.dir/src/mbot/timesync.cpp.i

mbot_autonomy/CMakeFiles/timesync.dir/src/mbot/timesync.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/timesync.dir/src/mbot/timesync.cpp.s"
	cd /home/pi/botlab-w23/mbot/mbot_autonomy/mbot_autonomy && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/botlab-w23/mbot/mbot_autonomy/src/mbot/timesync.cpp -o CMakeFiles/timesync.dir/src/mbot/timesync.cpp.s

# Object files for target timesync
timesync_OBJECTS = \
"CMakeFiles/timesync.dir/src/mbot/timesync.cpp.o"

# External object files for target timesync
timesync_EXTERNAL_OBJECTS =

bin/timesync: mbot_autonomy/CMakeFiles/timesync.dir/src/mbot/timesync.cpp.o
bin/timesync: mbot_autonomy/CMakeFiles/timesync.dir/build.make
bin/timesync: common_utils/libcommon_utils.a
bin/timesync: /usr/local/lib/liblcm.so.1.4.0
bin/timesync: /usr/lib/aarch64-linux-gnu/libglib-2.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libgobject-2.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libatk-1.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libgio-2.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libgthread-2.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libgmodule-2.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libgdk_pixbuf-2.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libcairo.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libharfbuzz.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libpango-1.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libpangocairo-1.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libpangoft2-1.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libpangoxft-1.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libgdk-x11-2.0.so
bin/timesync: /usr/lib/aarch64-linux-gnu/libgtk-x11-2.0.so
bin/timesync: mbot_lcm_msgs/libmbot_lcm_msgs.a
bin/timesync: /usr/local/lib/liblcm.so.1.4.0
bin/timesync: mbot_autonomy/CMakeFiles/timesync.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/botlab-w23/mbot/mbot_autonomy/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/timesync"
	cd /home/pi/botlab-w23/mbot/mbot_autonomy/mbot_autonomy && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/timesync.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mbot_autonomy/CMakeFiles/timesync.dir/build: bin/timesync

.PHONY : mbot_autonomy/CMakeFiles/timesync.dir/build

mbot_autonomy/CMakeFiles/timesync.dir/clean:
	cd /home/pi/botlab-w23/mbot/mbot_autonomy/mbot_autonomy && $(CMAKE_COMMAND) -P CMakeFiles/timesync.dir/cmake_clean.cmake
.PHONY : mbot_autonomy/CMakeFiles/timesync.dir/clean

mbot_autonomy/CMakeFiles/timesync.dir/depend:
	cd /home/pi/botlab-w23/mbot/mbot_autonomy && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/botlab-w23/mbot /home/pi/botlab-w23/mbot/mbot_autonomy /home/pi/botlab-w23/mbot/mbot_autonomy /home/pi/botlab-w23/mbot/mbot_autonomy/mbot_autonomy /home/pi/botlab-w23/mbot/mbot_autonomy/mbot_autonomy/CMakeFiles/timesync.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mbot_autonomy/CMakeFiles/timesync.dir/depend

