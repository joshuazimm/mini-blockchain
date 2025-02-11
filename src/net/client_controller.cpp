#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include "net/client_controller.h"
#include "net/consts.h"
#include "macros.h"

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
    int sock;
    CREATE_SOCKET(sock, AF_INET, SOCK_DGRAM, 0);

    logger->info("Listening on ip: " + get_local_ip());

    sockaddr_in client_addr;
    CREATE_INADDR_BROADCAST(client_addr);

    // Bind the socket
    ERROR_THROWING_SOCKET_OPERATION(
        bind(sock, (struct sockaddr*)&client_addr, sizeof(client_addr)), 
        sock, 
        "Failed to bind socket"
    );

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

                break;
            }
        }

        // Sleep for 5 seconds
        sleep(5);
    }

    close(sock);

    // Start TCP connection
    CREATE_SOCKET(sock, AF_INET, SOCK_STREAM, 0);

    // Set server address port
    server_address.sin_port = htons(NET_TCP_PORT);
    
    // Connect to the server
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        logger->error("Failed to connect to server");
        close(sock);
        return;
    }

    // Recieve data from the server
    while (true) {
        if (!running_thread) {
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            logger->info("Received " + std::to_string(bytes_received) + " bytes from server: " + std::string(buffer));
        } else {
            logger->error("Server disconnected");
            break;
        }

        // Sleep for 5 seconds
        sleep(5);
    }

    // Close the socket
    close(sock);
}