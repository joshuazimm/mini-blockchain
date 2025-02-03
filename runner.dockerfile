# Use a minimal Alpine image
FROM alpine:latest

# Set working directory inside the container
WORKDIR /app

# Copy the compiled binary from the host system
COPY my_binary /app/my_binary

# Ensure the binary has execution permissions
RUN chmod +x /app/my_binary

# Run the binary
CMD ["/app/my_binary"]