# Stop on errors
$ErrorActionPreference = "Stop"

# Navigate to the project root directory
Set-Location -Path (Get-Location)

Write-Host "Building the C++ compiler container..."
docker build -t cpp-builder -f builder.dockerfile .

Write-Host "Compiling the C++ binary..."
docker run -d --name cpp-builder-running cpp-builder
docker cp cpp-builder-running:/src/build/my_binary "$PWD"

docker stop cpp-builder-running
docker rm cpp-builder-running

Write-Host "Building the minimal runner container..."
docker build -t cpp-runner -f runner.dockerfile .

Write-Host "Spawning 5 runner containers..."
1..5 | ForEach-Object {
    $containerName = "cpp-runner-$_"
    docker run -d --name $containerName -d cpp-runner
    
    # Capture logs
    Write-Host "Logs for container ${containerName}:"
    docker logs $containerName
    docker stop $containerName
    docker rm $containerName
}

Write-Host "All runner containers' logs have been captured!"
Write-Host "All runner containers are now running!"
