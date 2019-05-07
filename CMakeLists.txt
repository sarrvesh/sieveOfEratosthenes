cmake_minimum_required(VERSION 3.5)
project(sieveOfEratosthenes C)

# Find externam packages
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
FIND_PACKAGE(GMP REQUIRED)

# Set directories
include_directories(${GMP_INCLUDES})
include_directories(${PROJECT_SOURCE_DIR}/src/)

# Enable optimization
set(CMAKE_C_STANDARD 11)
LIST(APPEND CMAKE_C_FLAGS "-O2 -march=native")

# Compile the sources
set(SIEVE ${PROJECT_SOURCE_DIR}/src/sieve.c ${PROJECT_SOURCE_DIR}/src/eratosthenes.c ${PROJECT_SOURCE_DIR}/src/common.c)
add_executable(sieve ${SIEVE})
target_link_libraries(sieve ${GMP_LIBRARIES})
