# Stop on errors
$ErrorActionPreference = "Stop"

Write-Host "Step 1: Building and running the builder container..."
docker compose up cpp-builder

Write-Host "Step 2: Removing the builder container (cleanup)..."
docker compose rm -s -v cpp-builder

Write-Host "Step 3: Starting the runner containers..."
docker compose up -d cpp-runner

Write-Host "Step 4: Waiting for the containers to stabilize..."
Start-Sleep -Seconds 3  # Allow some time for logs to generate

Write-Host "Step 5: Capturing logs from runner containers..."
docker compose logs

Write-Host "All processes completed successfully!"