# Use Alpine as the base image
FROM alpine:latest AS builder

# Install necessary dependencies
RUN apk add --no-cache g++ make cmake openssl-dev

# Set working directory inside the container
WORKDIR /src

# Copy entry script to the container
COPY ./builder_entrypoint.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh

ENTRYPOINT [ "/bin/sh", "/entrypoint.sh" ]