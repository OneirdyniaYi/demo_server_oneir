# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/yi/demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yi/demo/build

# Include any dependencies generated for this target.
include CMakeFiles/clientlib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/clientlib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clientlib.dir/flags.make

Demo.pb.h: ../protosrc/Demo.proto
Demo.pb.h: /usr/local/bin/protoc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running cpp protocol buffer compiler on protosrc/Demo.proto"
	/usr/local/bin/protoc --cpp_out /home/yi/demo/build -I /home/yi/demo/protosrc /home/yi/demo/protosrc/Demo.proto

Demo.pb.cc: Demo.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate Demo.pb.cc

ProtoID.pb.h: ../protosrc/ProtoID.proto
ProtoID.pb.h: /usr/local/bin/protoc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Running cpp protocol buffer compiler on protosrc/ProtoID.proto"
	/usr/local/bin/protoc --cpp_out /home/yi/demo/build -I /home/yi/demo/protosrc /home/yi/demo/protosrc/ProtoID.proto

ProtoID.pb.cc: ProtoID.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate ProtoID.pb.cc

CMakeFiles/clientlib.dir/common/buffer.cc.o: CMakeFiles/clientlib.dir/flags.make
CMakeFiles/clientlib.dir/common/buffer.cc.o: ../common/buffer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/clientlib.dir/common/buffer.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientlib.dir/common/buffer.cc.o -c /home/yi/demo/common/buffer.cc

CMakeFiles/clientlib.dir/common/buffer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientlib.dir/common/buffer.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yi/demo/common/buffer.cc > CMakeFiles/clientlib.dir/common/buffer.cc.i

CMakeFiles/clientlib.dir/common/buffer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientlib.dir/common/buffer.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yi/demo/common/buffer.cc -o CMakeFiles/clientlib.dir/common/buffer.cc.s

CMakeFiles/clientlib.dir/common/epoll.cc.o: CMakeFiles/clientlib.dir/flags.make
CMakeFiles/clientlib.dir/common/epoll.cc.o: ../common/epoll.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/clientlib.dir/common/epoll.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientlib.dir/common/epoll.cc.o -c /home/yi/demo/common/epoll.cc

CMakeFiles/clientlib.dir/common/epoll.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientlib.dir/common/epoll.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yi/demo/common/epoll.cc > CMakeFiles/clientlib.dir/common/epoll.cc.i

CMakeFiles/clientlib.dir/common/epoll.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientlib.dir/common/epoll.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yi/demo/common/epoll.cc -o CMakeFiles/clientlib.dir/common/epoll.cc.s

CMakeFiles/clientlib.dir/common/msghead.cc.o: CMakeFiles/clientlib.dir/flags.make
CMakeFiles/clientlib.dir/common/msghead.cc.o: ../common/msghead.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/clientlib.dir/common/msghead.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientlib.dir/common/msghead.cc.o -c /home/yi/demo/common/msghead.cc

CMakeFiles/clientlib.dir/common/msghead.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientlib.dir/common/msghead.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yi/demo/common/msghead.cc > CMakeFiles/clientlib.dir/common/msghead.cc.i

CMakeFiles/clientlib.dir/common/msghead.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientlib.dir/common/msghead.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yi/demo/common/msghead.cc -o CMakeFiles/clientlib.dir/common/msghead.cc.s

CMakeFiles/clientlib.dir/common/socket.cc.o: CMakeFiles/clientlib.dir/flags.make
CMakeFiles/clientlib.dir/common/socket.cc.o: ../common/socket.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/clientlib.dir/common/socket.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientlib.dir/common/socket.cc.o -c /home/yi/demo/common/socket.cc

CMakeFiles/clientlib.dir/common/socket.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientlib.dir/common/socket.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yi/demo/common/socket.cc > CMakeFiles/clientlib.dir/common/socket.cc.i

CMakeFiles/clientlib.dir/common/socket.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientlib.dir/common/socket.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yi/demo/common/socket.cc -o CMakeFiles/clientlib.dir/common/socket.cc.s

CMakeFiles/clientlib.dir/Demo.pb.cc.o: CMakeFiles/clientlib.dir/flags.make
CMakeFiles/clientlib.dir/Demo.pb.cc.o: Demo.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/clientlib.dir/Demo.pb.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientlib.dir/Demo.pb.cc.o -c /home/yi/demo/build/Demo.pb.cc

CMakeFiles/clientlib.dir/Demo.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientlib.dir/Demo.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yi/demo/build/Demo.pb.cc > CMakeFiles/clientlib.dir/Demo.pb.cc.i

CMakeFiles/clientlib.dir/Demo.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientlib.dir/Demo.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yi/demo/build/Demo.pb.cc -o CMakeFiles/clientlib.dir/Demo.pb.cc.s

CMakeFiles/clientlib.dir/ProtoID.pb.cc.o: CMakeFiles/clientlib.dir/flags.make
CMakeFiles/clientlib.dir/ProtoID.pb.cc.o: ProtoID.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/clientlib.dir/ProtoID.pb.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientlib.dir/ProtoID.pb.cc.o -c /home/yi/demo/build/ProtoID.pb.cc

CMakeFiles/clientlib.dir/ProtoID.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientlib.dir/ProtoID.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yi/demo/build/ProtoID.pb.cc > CMakeFiles/clientlib.dir/ProtoID.pb.cc.i

CMakeFiles/clientlib.dir/ProtoID.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientlib.dir/ProtoID.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yi/demo/build/ProtoID.pb.cc -o CMakeFiles/clientlib.dir/ProtoID.pb.cc.s

CMakeFiles/clientlib.dir/client/client.cc.o: CMakeFiles/clientlib.dir/flags.make
CMakeFiles/clientlib.dir/client/client.cc.o: ../client/client.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/clientlib.dir/client/client.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientlib.dir/client/client.cc.o -c /home/yi/demo/client/client.cc

CMakeFiles/clientlib.dir/client/client.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientlib.dir/client/client.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yi/demo/client/client.cc > CMakeFiles/clientlib.dir/client/client.cc.i

CMakeFiles/clientlib.dir/client/client.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientlib.dir/client/client.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yi/demo/client/client.cc -o CMakeFiles/clientlib.dir/client/client.cc.s

# Object files for target clientlib
clientlib_OBJECTS = \
"CMakeFiles/clientlib.dir/common/buffer.cc.o" \
"CMakeFiles/clientlib.dir/common/epoll.cc.o" \
"CMakeFiles/clientlib.dir/common/msghead.cc.o" \
"CMakeFiles/clientlib.dir/common/socket.cc.o" \
"CMakeFiles/clientlib.dir/Demo.pb.cc.o" \
"CMakeFiles/clientlib.dir/ProtoID.pb.cc.o" \
"CMakeFiles/clientlib.dir/client/client.cc.o"

# External object files for target clientlib
clientlib_EXTERNAL_OBJECTS =

../lib/libclientlib.a: CMakeFiles/clientlib.dir/common/buffer.cc.o
../lib/libclientlib.a: CMakeFiles/clientlib.dir/common/epoll.cc.o
../lib/libclientlib.a: CMakeFiles/clientlib.dir/common/msghead.cc.o
../lib/libclientlib.a: CMakeFiles/clientlib.dir/common/socket.cc.o
../lib/libclientlib.a: CMakeFiles/clientlib.dir/Demo.pb.cc.o
../lib/libclientlib.a: CMakeFiles/clientlib.dir/ProtoID.pb.cc.o
../lib/libclientlib.a: CMakeFiles/clientlib.dir/client/client.cc.o
../lib/libclientlib.a: CMakeFiles/clientlib.dir/build.make
../lib/libclientlib.a: CMakeFiles/clientlib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yi/demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX static library ../lib/libclientlib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/clientlib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clientlib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clientlib.dir/build: ../lib/libclientlib.a

.PHONY : CMakeFiles/clientlib.dir/build

CMakeFiles/clientlib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clientlib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clientlib.dir/clean

CMakeFiles/clientlib.dir/depend: Demo.pb.h
CMakeFiles/clientlib.dir/depend: Demo.pb.cc
CMakeFiles/clientlib.dir/depend: ProtoID.pb.h
CMakeFiles/clientlib.dir/depend: ProtoID.pb.cc
	cd /home/yi/demo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yi/demo /home/yi/demo /home/yi/demo/build /home/yi/demo/build /home/yi/demo/build/CMakeFiles/clientlib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clientlib.dir/depend

