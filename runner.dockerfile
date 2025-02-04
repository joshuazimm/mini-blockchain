# Use a minimal Alpine image
FROM alpine:latest

# Get dependencies 
RUN apk update && apk add --no-cache libstdc++