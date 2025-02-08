#ifndef P_OS_SRC_NET_NET_MSG_H
#define P_OS_SRC_NET_NET_MSG_H

#define MAX_MSG_DATA_SIZE 256

enum msg_type
{
    SERVER_DISCOVERY,
    ESTABLISH_CONNECTION,
    PAUSE_MINING,
    RESUME_MINING,
};

struct net_msg
{
    msg_type type;
    char data[MAX_MSG_DATA_SIZE];
};


#endif /* P_OS_SRC_NET_NET_MSG_H */
