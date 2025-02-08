#ifndef P_OS_SRC_NET_MACROS_H
#define P_OS_SRC_NET_MACROS_H

#include "net/consts.h"

#define SET_BROADCAST_ADDRESS(broadcast_address)              \
    memset(&broadcast_address, 0, sizeof(broadcast_address)); \
    broadcast_address.sin_family = AF_INET;                   \
    broadcast_address.sin_port = htons(NET_BROADCAST_PORT);

#define CREATE_SOCKET(socket_var, domain, type, protocol)                             \
    do                                                                                \
    {                                                                                 \
        socket_var = socket(domain, type, protocol);                                  \
        if (socket_var == -1)                                                         \
        {                                                                             \
            logger->error("Failed to create socket");                                 \
            return;                                                                   \
        }                                                                             \
    } while (0)

#define CREATE_INADDR_BROADCAST(broadcast_address) \
    SET_BROADCAST_ADDRESS(broadcast_address);       \
    broadcast_address.sin_addr.s_addr = INADDR_BROADCAST;

#define CREATE_INADDR_ANY(server_address) \
    SET_BROADCAST_ADDRESS(server_address); \
    server_address.sin_addr.s_addr = INADDR_ANY;

#define ERROR_THROWING_SOCKET_OPERATION(operation, socket, message) \
    if (operation < 0)                                               \
    {                                                               \
        logger->error(message);                                     \
        close(socket);                                               \
        return;                                                     \
    }

    
#endif /* P_OS_SRC_NET_MACROS_H */
