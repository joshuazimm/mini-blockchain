# Use a minimal Alpine image
FROM alpine:latest

# Get dependencies 
RUN apk update && apk add --no-cache libstdc++

# Copy the compiled binary from the host system
COPY mini_blockchain mini_blockchain

# Ensure the binary has execution permissions
RUN chmod +x mini_blockchain

# Run the binary
CMD ["/mini_blockchain"]