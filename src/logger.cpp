#include <iostream>
#include <thread>
#include <fstream>
#include "logger.h"


std::mutex Logger::_mtx;

Logger &Logger::getLogger(){

    std::lock_guard<std::mutex> lck(Logger::_mtx);

    static Logger _logger = Logger(3, "logger.txt");
    return _logger;
}

Logger::~Logger(){
    std::cout << "::Logger -  Thread Destroyed - flush all events (" << Logger::events.size() << ")" << std::endl;
    Logger::flush();
}

void Logger::printLoggerStartMsg(){
    std::lock_guard<std::mutex> lck(Logger::_mtx);
    std::cout << "::Logger - Thread Started: " << std::endl;
    std::cout << "::Logger - updateFrequencySec - " << Logger::updateFrequencySec << std::endl;
    std::cout << "::Logger -logPath - " << Logger::logPath << std::endl;
}

void Logger::printEvents(){
    std::cout << "::Logger - Event List: " << std::endl;
    std::lock_guard<std::mutex> lck(Logger::_mtx);

    std::for_each(Logger::events.begin(), Logger::events.end(), [](std::string evt){
            std::cout << evt << std::endl;
    });
}

void Logger::flush(){
    // copy all events
    std::unique_lock<std::mutex> lck(_mtx);
    std::vector<std::string> _events = std::move(Logger::events);
    Logger::events.clear();
    lck.unlock();

    std::lock_guard<std::mutex> lck2(_write_mtx);
    std::ofstream out;
    out.open(Logger::logPath, std::ios_base::app);
    for_each(_events.begin(), _events.end(), [&out](std::string _evt){
        out << _evt << std::endl;
    });
}

void Logger::main(bool &running){
    // open log
    Logger::printLoggerStartMsg();
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    while (running){
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count();

        if (timeSinceLastUpdate >= Logger::updateFrequencySec){
            // update Log file
            Logger::printEvents();
            std::cout << "::Logger - update Log - last update " << timeSinceLastUpdate << " sec ago" << std::endl;
            Logger::flush();

            start = std::chrono::system_clock::now();
            
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

std::string directionToStr(Snake::Direction direction){
    switch(direction) {
        case Snake::Direction::kUp: return "Up";
        case Snake::Direction::kDown: return "Down";
        case Snake::Direction::kLeft: return "Left";
        case Snake::Direction::kRight: return "Right";
        default:
            throw std::invalid_argument("direction value not handled");
    }
}

std::string timeToStr(std::chrono::time_point<std::chrono::system_clock> time){
    std::time_t t = std::chrono::system_clock::to_time_t(time);
    std::string t2 = std::string(std::ctime(&t));
    t2.pop_back();
    return t2;
}

void Logger::logDirectionChange(Snake::Direction direction){
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::string log = "--" + timeToStr(now)+ "\t-- GO: " + directionToStr(direction);

    std::lock_guard<std::mutex> lck(Logger::_mtx);
    events.push_back(std::move(log));
}

void Logger::logEatEvent(){
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::string log = "--" + timeToStr(now)+ "\t-- YUMMY!";

    std::lock_guard<std::mutex> lck(Logger::_mtx);
    events.push_back(std::move(log));
}


void Logger::logDead(){
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::string log = "--" + timeToStr(now)+ "\t-- DEAD";

    std::lock_guard<std::mutex> lck(Logger::_mtx);
    events.push_back(std::move(log));
}


