#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <filesystem>
#include <mutex>
#include "snake.h"


class Logger {
public:
    Logger(int updateFrequencySec, std::string logPath): 
        updateFrequencySec(updateFrequencySec), logPath(logPath) {};
    ~Logger();
    
    void logDirectionChange(Snake::Direction direction);
    void logEatEvent();
    void logDead();

    void main(bool &running); // main loop for the logger
private:
    void flush(); // flush the events log to file

private:
    std::vector<std::string> events; // internal ordered list of event
    std::string logPath; // path to log file
    long updateFrequencySec; // how often to write to file

    std::mutex mtx; // protect events structure
    void printStartMsg();
};

#endif