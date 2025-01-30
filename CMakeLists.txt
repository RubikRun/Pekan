cmake_minimum_required(VERSION 3.6...3.31)

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set avaiable configurations to Debug and Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

# Create project Pekan
project(Pekan)

# Make project Pekan compile following source file into an executable
add_executable(Pekan src/Pekan/main.cpp)

# Set project Pekan to be the startup project by default
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Pekan)
