cmake_minimum_required(VERSION 2.8.11)
project(VisualizeParticleFilter)
find_package( OpenCV REQUIRED )

#set(SRCS src/main.cpp src/particle_filter.cpp)
set(SRCS src/main.cpp)
set_source_files_properties(${SRCS} PROPERTIES COMPILE_FLAGS -std=c++0x)

# Create the executable
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(VisualizeParticleFilter ${SRCS})
target_link_libraries( VisualizeParticleFilter ${OpenCV_LIBS} )
