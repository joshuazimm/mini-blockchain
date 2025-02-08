#ifndef P_OS_NET_MASTER_CONTROLLER_H
#define P_OS_NET_MASTER_CONTROLLER_H

#include "net_controller.h"
#include <vector>

class MasterController: public NetController {
private:
  virtual void start_broadcast();

  void start_broadcasting();
  void start_accepting_clients();

  std::vector<int> client_sockets;
  std::vector<std::string> message_queue;

public:
  MasterController();
  void send_message_to_clients(const char *message, size_t message_size);
};

#endif /* P_OS_NET_MASTER_CONTROLLER_H */
