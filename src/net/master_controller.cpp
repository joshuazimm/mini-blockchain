#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "net/master_controller.h"
#include "net/consts.h"
#include "macros.h"

MasterController::MasterController() {
    // Initialize the logger
    logger->set_app_category("MasterController");
}

void MasterController::start_broadcasting() {
    // Get socket
    int sock;
    CREATE_SOCKET(sock, AF_INET, SOCK_DGRAM, 0);

    // Set socket options (enable broadcast)
    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        logger->error("Failed to set socket options");
        close(sock);
        return;
    }
    // Set server address
    struct sockaddr_in server_address;
    CREATE_INADDR_BROADCAST(server_address);

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

    close(sock);
}

void MasterController::start_accepting_clients() {
    // Get socket
    int sock;
    CREATE_SOCKET(sock, AF_INET, SOCK_STREAM, 0);

    // Set server address
    struct sockaddr_in server_address;
    CREATE_INADDR_ANY(server_address);
    server_address.sin_port = htons(NET_TCP_PORT);

    // Bind the socket
    ERROR_THROWING_SOCKET_OPERATION(
        bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)), 
        sock, 
        "Failed to bind socket"
    );

    // Listen on the socket
    ERROR_THROWING_SOCKET_OPERATION(
        listen(sock, 5),
        sock,
        "Failed to listen on socket"
    );

    // Listen for incoming messages
    while (true) {
        if (!running_thread) {
            break;
        }

        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_sock = accept(sock, (struct sockaddr*)&client_address, &client_address_size);
        if (client_sock < 0) {
            logger->error("Failed to accept client");
            continue;;
        }

        // Add the client to the list
        client_sockets.push_back(client_sock);

        logger->info("Client with address " + std::string(inet_ntoa(client_address.sin_addr)) + " connected");
    }
}

void MasterController::start_broadcast() {
    logger->info("Starting broadcast for master server");

    // Fork for broadcasting and accepting clients
    running_thread = true;
    std::thread broadcast_thread(&MasterController::start_broadcasting, this);
    std::thread accept_thread(&MasterController::start_accepting_clients, this);

    // Wait for the threads to finish
    broadcast_thread.join();
    accept_thread.join();
}

void MasterController::send_message_to_clients(const char *message, size_t message_size) {
    for (int client_sock : client_sockets) {
        logger->info("Sending message '" + std::string(message) + "' to client with socket " + std::to_string(client_sock));
        if (send(client_sock, message, message_size, 0) < 0) {
            logger->error("Failed to send message to client");
        }
    }

    // // Recieve response from clients
    // for (int client_sock : client_sockets) {
    //     char response[1024];
    //     if (recv(client_sock, response, 1024, 0) < 0) {
    //         logger->error("Failed to recieve response from client");
    //     } else {
    //         logger->info("Response from client: " + std::string(response));
    //     }
    // }
}