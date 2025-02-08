#ifndef P_OS_NET_MASTER_CONTROLLER_H
#define P_OS_NET_MASTER_CONTROLLER_H

#include "../net_controller.h"
#include "../consts.h"
#include <vector>

class MasterController: public NetController {
private:
  virtual void start_broadcast();
  std::vector<sockaddr_in> clients;

public:
  MasterController();
};

#endif /* P_OS_NET_MASTER_CONTROLLER_H */
