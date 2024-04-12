#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <filesystem>
#include <mutex>
#include "snake.h"


/*
 The logger class follows the Singleton Pattern. Only one instance is allowed throughout the
 lifetime of the program, and the instance can be accessed through Logger#getLogger(). 
 The logger will run as a thread in the background, and will keep a log records of events
 happening in the game, storing them in an internal vector `events`. Every `updateFrequencySec` seconds,
 the thread will flush all events to the log file `logPath`. All access to shared data is protected
 by mutex
*/
class Logger {
public:    
    // Constructor/destructors
    Logger(Logger &Logger) = delete; // delete copy Constructor 
    void operator=(const Logger &) = delete; // delete copy assignment
    Logger(Logger &&Logger) = delete; // delete move Constructor 
    void operator=(const Logger &&) = delete; // delete move assignment

public:
    // Get the Singleton logger object
    static Logger& getLogger();

    // main loop for the logger
    void main(bool &running); 

    void logDirectionChange(const Snake::Direction direction);
    void logEatEvent(const int score, const int size, const float speed);
    void logDead();

private:
    // flush the events log to file
    void flush();

private:
    ~Logger();
    // private constructor for the singleton
    Logger(int updateFrequencySec, std::string logPath): 
        updateFrequencySec(updateFrequencySec), logPath(logPath) {}; 

    // Priting methods
    void printLoggerStartMsg();
    void printEvents();

private: 
    std::vector<std::string> events; // internal ordered list of event
    std::string logPath; // path to log file
    const long updateFrequencySec; // how often to write to file

    static std::mutex _mtx; // protect events structure
    std::mutex _write_mtx; // protect events structure        
};

#endif