
cmake_minimum_required(VERSION 3.4)

# Setup project
project(cem CXX C)
set(TEST_BIN cem-test)

# Default to debug build
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Debug)
endif()

# Set compiler options
add_definitions(-Wall -Wpedantic -Wno-unused-function -Wno-c11-extensions)

set(CMAKE_C_FLAGS_DEBUG "-std=gnu11 -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-std=gnu++11 -O0 -g")

set(CMAKE_C_FLAGS_RELEASE "-std=gnu11 -Os -flto")
set(CMAKE_CXX_FLAGS_RELEASE "-std=gnu++11 -Os -flto")

# Add google mock and test runner
include(test/gtest.cmake)
set(TEST_SRC test/test.cpp)

# Add submodules
include(memqueue/memqueue.cmake)

# Include project directory
include_directories(
    ${PROJECT_SOURCE_DIR}
)

# Build test executable
add_executable(${TEST_BIN} ${TEST_SRC})
target_link_libraries(${TEST_BIN} ${CEM_LIBS} gtest pthread stdc++)
add_dependencies(${TEST_BIN} ${CEM_LIBS})
