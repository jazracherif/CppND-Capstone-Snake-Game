#include <iostream>
#include <thread>
#include <fstream>
#include "gameLogger.h"
#include <algorithm>
#include "util.h"

std::mutex GameLogger::_mtx;

std::string directionToStr(const Snake::Direction direction){
    switch(direction) {
        case Snake::Direction::kUp: return "Up";
        case Snake::Direction::kDown: return "Down";
        case Snake::Direction::kLeft: return "Left";
        case Snake::Direction::kRight: return "Right";
        default:
            throw std::invalid_argument("direction value not handled");
    }
}


GameLogger &GameLogger::getLogger(){
    // should i use std::call_once?
    static GameLogger _logger = GameLogger(3, "game_logger.txt");
    return _logger;
}

GameLogger::~GameLogger(){
    std::cout << "::Logger -  Thread Destroyed - flush all events (" << GameLogger::events.size() << ")" << std::endl;
    GameLogger::flush();
}


void GameLogger::flush(){
    // copy all events
    std::unique_lock<std::mutex> lck(_mtx);
    std::vector<std::string> _events = std::move(GameLogger::events);
    GameLogger::events.clear();
    lck.unlock();

    std::lock_guard<std::mutex> lck2(_write_mtx);
    std::ofstream out;
    out.open(GameLogger::logPath, std::ios_base::app);
    for_each(_events.begin(), _events.end(), [&out](std::string _evt){
        out << _evt << std::endl;
    });
}

void GameLogger::main(bool &running){
    // open log
    GameLogger::printLoggerStartMsg();
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    while (running && !GameLogger::stopped){
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count();

        if (timeSinceLastUpdate >= GameLogger::updateFrequencySec){
            // flush all events
            std::cout << "::Logger - update Log - last update " << timeSinceLastUpdate << " sec ago" << std::endl;
            GameLogger::printEvents();
            GameLogger::flush();

            start = std::chrono::system_clock::now();
            
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    GameLogger::flush();
}


void GameLogger::logDirectionChange(const Snake::Direction direction){
    if (GameLogger::stopped)
        return;

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::string log = "--" + Util::timeToStr(now)+ "\t-- GO: " + directionToStr(direction) + " --";

    std::lock_guard<std::mutex> lck(GameLogger::_mtx);
    events.push_back(std::move(log));
}

void GameLogger::logEatEvent(const int score, const int size, const float speed){
    if (GameLogger::stopped)
        return;
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::string log = "--" + Util::timeToStr(now)+ "\t-- YUMMY! " + " score: " +\
                        std::to_string(score)+ " size: " +\
                        std::to_string(size) + " speed: " +\
                        std::to_string(speed) + " --";

    std::lock_guard<std::mutex> lck(GameLogger::_mtx);
    events.push_back(std::move(log));
}


void GameLogger::logDead(){
    if (GameLogger::stopped)
        return;

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::string log = "--" + Util::timeToStr(now)+ "\t-- DEAD --";

    std::lock_guard<std::mutex> lck(GameLogger::_mtx);
    events.push_back(std::move(log));
    GameLogger::stopped = true;
}





void GameLogger::printLoggerStartMsg(){
    std::lock_guard<std::mutex> lck(GameLogger::_mtx);
    std::cout << "::GameLogger - Thread Started: " << std::endl;
    std::cout << "::GameLogger - updateFrequencySec - " << GameLogger::updateFrequencySec << std::endl;
    std::cout << "::GameLogger -logPath - " << GameLogger::logPath << std::endl;
}


void GameLogger::printEvents(){
    std::cout << "::GameLogger - Event List: " << std::endl;
    std::lock_guard<std::mutex> lck(GameLogger::_mtx);

    std::for_each(GameLogger::events.begin(), GameLogger::events.end(), [](std::string evt){
            std::cout << evt << std::endl;
    });
}