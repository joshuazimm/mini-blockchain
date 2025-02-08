#ifndef P_OS_NET_MASTER_CONTROLLER_H
#define P_OS_NET_MASTER_CONTROLLER_H

#include "../net_controller.h"

class MasterController: public NetController {
private:
  virtual void start_broadcast();

public:
  MasterController();
};

#endif /* P_OS_NET_MASTER_CONTROLLER_H */
