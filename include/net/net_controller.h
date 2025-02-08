#ifndef P_OS_NET_CONTROLLER_H
#define P_OS_NET_CONTROLLER_H

#include "logger/logger.h"
#include <thread>

class NetController {
protected:
  Logger *logger = new Logger("NetController");
  virtual void start_broadcast() = 0;
  std::thread broadcast_thread;
  
  bool running_thread = false;

public:
  // TODO: Implement test case that attempts to create a NetController object and destroy it to make sure the logger
  // object is properly destroyed and no memory leaks occur
  ~NetController();

  /**
   * @brief Start broadcasting the server discovery message
   * WARNING: This method is unsafe network wise and should only be used in controlled private enviornments. In
   * larger enviornments, a predefined server address should be set.
   */
  void start_broadcast_thread();
  void join_thread();
};

#endif /* P_OS_NET_MASTER_CONTROLLER_H */
