cmake_minimum_required(VERSION 2.8.11)
project(VIS_PARTICLE_FILTER)


# Build the particle filter project and solution.
# Use C++11
#set(SRCS src/main.cpp src/vis_particle_filter.cpp)
set(SRCS src/main.cpp)
set_source_files_properties(${SRCS} PROPERTIES COMPILE_FLAGS -std=c++0x)

# Create the executable
add_executable(vis_particle_filter ${SRCS})
