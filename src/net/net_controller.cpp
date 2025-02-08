#include "net/net_controller.h"
#include <unistd.h>

NetController::~NetController() {
    if (broadcast_thread.joinable()) {
        running_thread = false;
        broadcast_thread.join();
    }

    delete logger;
}

void NetController::start_broadcast_thread() {
    running_thread = true;
    broadcast_thread = std::thread([this] { 
        while (true)
        {
            start_broadcast(); 
        }
    });
}

void NetController::join_thread() {
    if (broadcast_thread.joinable()) {
        broadcast_thread.join();
    }
}