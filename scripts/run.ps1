# Stop on errors
$ErrorActionPreference = "Stop"

# Navigate to the project root directory
Set-Location -Path (Get-Location)

Write-Host "Creating a custom Docker network..."
docker network create my_network

Write-Host "Building the C++ compiler container..."
docker build -t cpp-builder -f builder.dockerfile .

Write-Host "Compiling the C++ binary..."
docker run -d --name cpp-builder-running --network my_network cpp-builder
docker cp cpp-builder-running:/src/build/mini_blockchain "$PWD"

docker stop cpp-builder-running
docker rm cpp-builder-running

Write-Host "Building the minimal runner container..."
docker build -t cpp-runner -f runner.dockerfile .

Write-Host "Spawning 5 runner containers..."
1..5 | ForEach-Object {
    $containerName = "cpp-runner-$_"
    
    # Start the container with the custom network
    docker run -d --name $containerName --network my_network cpp-runner
}

Write-Host "All runner containers are now running on the custom network!"

# Capture logs
1..5 | ForEach-Object {
    $containerName = "cpp-runner-$_"
    Write-Host "Logs for container ${containerName}:"
    docker logs $containerName
}

Write-Host "All runner containers' logs have been captured!"