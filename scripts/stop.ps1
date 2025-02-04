Write-Host "Deleting all Docker containers..."

$containers = docker ps -a -q

# Stop all running containers if any exist
if ($containers) {
    $containers | ForEach-Object { docker stop $_ }
    $containers | ForEach-Object { docker rm $_ }
    Write-Host "All runner containers have been deleted!"
} else {
    Write-Host "No containers found to delete."
}