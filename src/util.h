#ifndef UTIL_H
#define UTIL_H

#include <chrono>
#include <string>

class Util {
public:
    static std::string timeToStr(std::chrono::time_point<std::chrono::system_clock> time){
        std::time_t t = std::chrono::system_clock::to_time_t(time);
        std::string t2 = std::string(std::ctime(&t));
        t2.pop_back();
        return t2;
    }
};

#endif
