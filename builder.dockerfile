# Use Alpine as the base image
FROM alpine:latest AS builder

# Install necessary dependencies
RUN apk add --no-cache g++ make cmake openssl-dev

# Set working directory inside the container
WORKDIR /src

# Copy source files and CMakeLists.txt
COPY src/ /src/

# Create a build directory and compile
RUN mkdir build && cd build && cmake .. && make