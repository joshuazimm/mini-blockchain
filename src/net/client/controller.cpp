#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "controller.h"
#include "../consts.h"

ClientController::ClientController() {
    // Initialize the logger
    logger->set_app_category("ClientController");
}

std::string get_local_ip() {
    char hostname_buffer[1024];
    gethostname(hostname_buffer, sizeof(hostname_buffer));
    struct hostent* host = gethostbyname(hostname_buffer);
    if (host == NULL) {
        return "";
    }

    char ip_buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, host->h_addr_list[0], ip_buffer, INET_ADDRSTRLEN);

    return std::string(ip_buffer);
}

void ClientController::start_broadcast() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        logger->error("Failed to create socket");
        return;
    }

    logger->info("Listening on ip: " + get_local_ip());

    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(NET_BROADCAST_PORT);
    client_addr.sin_addr.s_addr = INADDR_BROADCAST;

    // Bind the socket
    if (bind(sock, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        logger->error("Failed to bind socket");
        close(sock);
        return;
    }

    char buffer[1024];
    sockaddr_in server_address;
    socklen_t server_address_len = sizeof(server_address);

    while (true) {
        if (!running_thread) {
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer), 0,
                                      (sockaddr*)&server_address, &server_address_len);
        if (bytes_received > 0) {
            logger->info("Received broadcast message with " + 
                std::to_string(bytes_received) + 
                " bytes from server " + 
                std::string(inet_ntoa(server_address.sin_addr)) + 
                " message: " + 
                std::string(buffer)
            );

            std::string message(buffer);
            if (message == SERVER_DISCOVERY_MESSAGE) {
                char server_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &server_address.sin_addr, server_ip, INET_ADDRSTRLEN);
                logger->debug("Attempting to connect to validated server: " + std::string(server_ip));

                // TODO: Establish TCP connection with server
                break;
            }
        }

        // Sleep for 5 seconds
        sleep(5);
    }

    // Close the socket
    close(sock);
}