# Use a minimal Alpine image
FROM alpine:latest

# Get dependencies 
RUN apk update && apk add --no-cache libstdc++

# Copy the compiled binary from the host system
COPY my_binary /output/my_binary

# Ensure the binary has execution permissions
RUN chmod +x /output/my_binary

# Run the binary
CMD ["/output/my_binary"]