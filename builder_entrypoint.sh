#!/bin/bash

# Create a build directory if it doesn't exist
mkdir -p /build

# Navigate to the build directory
cd /build

# Does /src exist?
if [ ! -d "/src" ]; then
  echo "Error: /src does not exist"
  exit 1
fi

# Run CMake to configure the project
cmake /src

# Build the project
make

# Does /output exist?
if [ ! -d "/output" ]; then
  echo "Error: /output does not exist"
  exit 1
fi

cp /build/mini_blockchain /output/
echo 'Binary copied'