# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jerry/nubot_ws/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jerry/nubot_ws/src-build

# Include any dependencies generated for this target.
include camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/depend.make

# Include the progress variables for this target.
include camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/progress.make

# Include the compile flags for this target's objects.
include camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/flags.make

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o: camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/flags.make
camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o: /home/jerry/nubot_ws/src/camera_driver/tiscamera/gscam/src/gscam.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jerry/nubot_ws/src-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o"
	cd /home/jerry/nubot_ws/src-build/camera_driver/tiscamera/gscam && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gscam.dir/src/gscam.cpp.o -c /home/jerry/nubot_ws/src/camera_driver/tiscamera/gscam/src/gscam.cpp

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gscam.dir/src/gscam.cpp.i"
	cd /home/jerry/nubot_ws/src-build/camera_driver/tiscamera/gscam && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jerry/nubot_ws/src/camera_driver/tiscamera/gscam/src/gscam.cpp > CMakeFiles/gscam.dir/src/gscam.cpp.i

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gscam.dir/src/gscam.cpp.s"
	cd /home/jerry/nubot_ws/src-build/camera_driver/tiscamera/gscam && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jerry/nubot_ws/src/camera_driver/tiscamera/gscam/src/gscam.cpp -o CMakeFiles/gscam.dir/src/gscam.cpp.s

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o.requires:

.PHONY : camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o.requires

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o.provides: camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o.requires
	$(MAKE) -f camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/build.make camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o.provides.build
.PHONY : camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o.provides

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o.provides.build: camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o


# Object files for target gscam
gscam_OBJECTS = \
"CMakeFiles/gscam.dir/src/gscam.cpp.o"

# External object files for target gscam
gscam_EXTERNAL_OBJECTS =

devel/lib/libgscam.so: camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o
devel/lib/libgscam.so: camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/build.make
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libimage_transport.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libmessage_filters.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libnodeletlib.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libuuid.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libbondcpp.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libtinyxml2.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libclass_loader.so
devel/lib/libgscam.so: /usr/lib/libPocoFoundation.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libdl.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libroslib.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/librospack.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libpython2.7.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_program_options.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libtinyxml.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libcamera_info_manager.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libcamera_calibration_parsers.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libroscpp.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_signals.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libxmlrpcpp.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libcv_bridge.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_core3.so.3.3.1
devel/lib/libgscam.so: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_imgproc3.so.3.3.1
devel/lib/libgscam.so: /opt/ros/kinetic/lib/x86_64-linux-gnu/libopencv_imgcodecs3.so.3.3.1
devel/lib/libgscam.so: /opt/ros/kinetic/lib/librosconsole.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_regex.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libdynamic_reconfigure_config_init_mutex.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libroscpp_serialization.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/librostime.so
devel/lib/libgscam.so: /opt/ros/kinetic/lib/libcpp_common.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_thread.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libpthread.so
devel/lib/libgscam.so: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
devel/lib/libgscam.so: camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jerry/nubot_ws/src-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../../devel/lib/libgscam.so"
	cd /home/jerry/nubot_ws/src-build/camera_driver/tiscamera/gscam && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gscam.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/build: devel/lib/libgscam.so

.PHONY : camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/build

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/requires: camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/src/gscam.cpp.o.requires

.PHONY : camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/requires

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/clean:
	cd /home/jerry/nubot_ws/src-build/camera_driver/tiscamera/gscam && $(CMAKE_COMMAND) -P CMakeFiles/gscam.dir/cmake_clean.cmake
.PHONY : camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/clean

camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/depend:
	cd /home/jerry/nubot_ws/src-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jerry/nubot_ws/src /home/jerry/nubot_ws/src/camera_driver/tiscamera/gscam /home/jerry/nubot_ws/src-build /home/jerry/nubot_ws/src-build/camera_driver/tiscamera/gscam /home/jerry/nubot_ws/src-build/camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : camera_driver/tiscamera/gscam/CMakeFiles/gscam.dir/depend
