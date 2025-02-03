#!/bin/sh

# Stop on any errors
set -e

# Move to the project root directory (assuming script is run from scripts/)
cd "$(dirname "$0")/.."

echo "Building the C++ compiler container..."
docker build -t cpp-builder -f builder.Dockerfile .

echo "Compiling the C++ binary..."
docker run --rm -v $(pwd)/output:/output cpp-builder

echo "Building the minimal runner container..."
docker build -t cpp-runner -f runner.Dockerfile .

echo "Spawning 5 runner containers..."
for i in $(seq 1 5); do
    docker run --rm --name cpp-runner-$i -d cpp-runner
done

echo "All runner containers are now running!"