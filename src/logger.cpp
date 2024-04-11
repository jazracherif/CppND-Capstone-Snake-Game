#include <iostream>
#include <thread>

#include "logger.h"


Logger::~Logger(){
    std::cout << "::Logger -  Thread Destroyed" << std::endl;
}

void Logger::printStartMsg(){
    std::cout << "::Logger - Thread Started: " << std::endl;
    std::cout << "::Logger - updateFrequencySec - " << Logger::updateFrequencySec << std::endl;
    std::cout << "::Logger -logPath - " << Logger::logPath << std::endl;
}

void Logger::main(bool &running){
    // open log
    printStartMsg();
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    while (running){
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count();

        if (timeSinceLastUpdate >= Logger::updateFrequencySec){
            // update Log file
            std::cout << "::Logger - update Log - last update " << timeSinceLastUpdate << " sec ago" << std::endl;
            start = std::chrono::system_clock::now();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


