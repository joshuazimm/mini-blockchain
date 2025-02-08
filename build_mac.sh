#!/bin/bash

# Create a build directory if it doesn't exist
mkdir -p build

# Navigate to the build directory
cd build

# Run CMake to configure the project
cmake ../src

# Build the project
make