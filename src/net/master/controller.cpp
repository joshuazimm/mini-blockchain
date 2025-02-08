#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "controller.h"
#include "../consts.h"

MasterController::MasterController() {
    // Initialize the logger
    logger->set_app_category("MasterController");
}

void MasterController::start_broadcast() {
    logger->info("Starting broadcast for master server");

    // Get socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        logger->error("Failed to create socket");
        return;
    }

    // Set socket options (enable broadcast)
    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        logger->error("Failed to set socket options");
        close(sock);
        return;
    }

    // Set server address
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(NET_BROADCAST_PORT);
    server_address.sin_addr.s_addr = INADDR_BROADCAST;

    while (true) {
        if (!running_thread) {
            break;
        }

        // Send broadcast message
        if (sendto(sock, SERVER_DISCOVERY_MESSAGE, sizeof(SERVER_DISCOVERY_MESSAGE), 0, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
            logger->error("Failed to send broadcast message");
            break;
        } else {
            logger->info("Broadcast message sent on server");
        }

        // Sleep for 5 seconds
        sleep(5);
    }

    // Close the socket
    close(sock);
}