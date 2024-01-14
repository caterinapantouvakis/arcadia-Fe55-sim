#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(arcadiaSim)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${ROOT_LIBRARY_DIR})

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
# find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include
		            ${ROOT_INCLUDE_DIRS})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h 
                  ${PROJECT_SOURCE_DIR}/include/*.hpp)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(arcadiaSim main.cpp ${sources} ${headers})
target_link_libraries(arcadiaSim ${ROOT_LIBRARIES})
