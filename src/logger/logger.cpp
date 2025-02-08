#include "logger/logger.h"
#include <chrono>
#include <iostream>
#include <iomanip>

void Logger::get_log_color(LOG_LEVEL level) {
    switch (level) {
        case INFO:
            std::cout << "\033[1;32m";
            break;
        case WARN:
            std::cout << "\033[1;33m";
            break;
        case ERROR:
            std::cout << "\033[1;31m";
            break;
        case DEBUG:
            std::cout << "\033[1;34m";
            break;
        case CRITICAL:
            std::cout << "\033[1;35m";
            break;
        default:
            std::cout << "\033[1;37m";
            break;
    }
}

static std::mutex log_mutex;

void Logger::log(const std::string& message, LOG_LEVEL level) {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::localtime(&now_time_t);

    get_log_color(level);
    
    // TODO: Make this more like a queue rather than a slow mutex
    log_mutex.lock();
    std::cout << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << " " << app_category << ": " << message << "\033[0m" << std::endl;
    log_mutex.unlock();
}