#!/bin/bash

echo "Stopping and removing all Docker containers..."

# Get all container IDs
containers=$(docker ps -aq)

if [ -n "$containers" ]; then
    # Stop all running containers
    echo "Stopping containers..."
    docker stop $containers

    # Remove all containers
    echo "Removing containers..."
    docker rm $containers

    echo "All containers have been stopped and deleted!"
else
    echo "No containers found."
fi