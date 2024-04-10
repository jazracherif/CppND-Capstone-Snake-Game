#include <iostream>
#include <thread>

#include "logger.h"


void Logger::main(bool &running){
    // open log
    std::cout << "Logger Thread Started" << std::endl;
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    while (running){
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count();

        if (timeSinceLastUpdate >= Logger::updateFrequencySec){
            // update Log file
            std::cout << "update Log" << std::endl;
            start = std::chrono::system_clock::now();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


