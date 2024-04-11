#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <cstddef>
#include <string>

class GameConfig {
public:
    GameConfig(std::string url = "");

    std::size_t kFramesPerSecond() { return _kFramesPerSecond; }
    std::size_t kMsPerFrame() { return _kMsPerFrame; }
    std::size_t kScreenWidth() { return _kScreenWidth; }
    std::size_t kScreenHeight() { return _kScreenHeight; }
    std::size_t kGridWidth() {return _kGridWidth; }
    std::size_t kGridHeight() { return _kGridHeight;}

private:
    void printConfig();
    std::string defaultConfigUrl();

    std::size_t _kFramesPerSecond{60};
    std::size_t _kMsPerFrame{1000 / _kFramesPerSecond};
    std::size_t _kScreenWidth{640};
    std::size_t _kScreenHeight{640};
    std::size_t _kGridWidth{32};
    std::size_t _kGridHeight{32};
};

#endif