#include <iostream>
#include <thread>
#include "controller.h"
#include "game.h"
#include "renderer.h"

#include "gameConfig.h"


int main (int argc, const char * argv[]) {
  std::string config_url;

  if (argc > 1){
    // First Argument is the config file url
    config_url = std::string(argv[1]);
  }
  GameConfig config(config_url);
  
  Renderer renderer(config.kScreenWidth(), config.kScreenHeight(), config.kGridWidth(), config.kGridHeight());
  Controller controller;

  Game game(config.kGridWidth(), config.kGridHeight());
  game.Run(controller, renderer, config.kMsPerFrame());

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}