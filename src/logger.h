#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <filesystem>
#include <mutex>
#include "snake.h"


// Singleton class
class Logger {
public:    
    Logger(Logger &Logger) = delete; // delete for singleton
    void operator=(const Logger &) = delete; // delete for singleton

    void logDirectionChange(Snake::Direction direction);
    void logEatEvent();
    void logDead();

    void main(bool &running); // main loop for the logger

    static Logger& getLogger();

private:
    void flush(); // flush the events log to file

private:
    ~Logger();
    Logger(int updateFrequencySec, std::string logPath): 
        updateFrequencySec(updateFrequencySec), logPath(logPath) {}; // private constructor
    void printLoggerStartMsg();
    void printEvents();

private: 
    std::vector<std::string> events; // internal ordered list of event
    std::string logPath; // path to log file
    long updateFrequencySec; // how often to write to file

    static std::mutex _mtx; // protect events structure
    std::mutex _write_mtx; // protect events structure

    // static std::unique_ptr<Logger> _logger;
    
};

#endif