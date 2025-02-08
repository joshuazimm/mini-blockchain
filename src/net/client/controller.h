#ifndef P_OS_NET_CLIENT_CONTROLLER_H
#define P_OS_NET_CLIENT_CONTROLLER_H

#include "../../logger/logger.h"
#include "../net_controller.h"
#include <thread>

class ClientController : public NetController {
private:
  virtual void start_broadcast();

public:
  ClientController();
};

#endif /* P_OS_NET_CLIENT_CONTROLLER_H */