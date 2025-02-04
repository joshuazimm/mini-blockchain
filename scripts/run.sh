#!/bin/bash

# Stop on errors
set -e

echo "Step 1: Building and running the builder container..."
docker compose up cpp-builder

echo "Step 2: Removing the builder container (cleanup)..."
docker compose rm -s -v cpp-builder

echo "Step 3: Starting the runner containers..."
docker compose up -d cpp-runner

echo "Step 4: Waiting for the containers to stabilize..."
sleep 3  # Allow some time for logs to generate

echo "Step 5: Capturing logs from runner containers..."
docker compose logs cpp-runner

echo "All processes completed successfully!"