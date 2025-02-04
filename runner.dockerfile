# Use a minimal Alpine image
FROM alpine:latest

WORKDIR /app

# Get dependencies 
RUN apk update && apk add --no-cache libstdc++

# Copy the binary from the build context to the container
COPY ./build/mini_blockchain /bin/mini_blockchain

# Ensure the binary is executable
RUN chmod +x /bin/mini_blockchain

# Set the command to run the binary when the container starts
ENTRYPOINT ["echo", "Hello World"]