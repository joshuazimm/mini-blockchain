#ifndef P_OS_SRC_LOGGER_LOGGER_H
#define P_OS_SRC_LOGGER_LOGGER_H

#include <mutex>
#include <string>

enum LOG_LEVEL { INFO, WARN, ERROR, DEBUG, CRITICAL };

class Logger {
private:
  void get_log_color(LOG_LEVEL level);
  void log(const std::string &message, LOG_LEVEL level = INFO);

  std::string app_category;

public:
  Logger(const std::string &app_category) : app_category(app_category) {}

  void operator<<(const std::string &message) { info(message); }
  void operator<<(const char *message) { info(message); }
  void set_app_category(const std::string &app_category) { this->app_category = app_category; }

  void error(const std::string &message) { log(message, ERROR); }
  void warn(const std::string &message) { log(message, WARN); }
  void debug(const std::string &message) { log(message, DEBUG); }
  void critical(const std::string &message) { log(message, CRITICAL); }
  void info(const std::string &message) { log(message, INFO); }
};

#endif /* P_OS_SRC_LOGGER_LOGGER_H */