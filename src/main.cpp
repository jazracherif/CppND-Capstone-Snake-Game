#include <iostream>
#include <thread>

#include "controller.h"
#include "game.h"
#include "renderer.h"

#include "gameConfig.h"


int main() {

  GameConfig config;
  
  Renderer renderer(config.kScreenWidth(), config.kScreenHeight(), config.kGridWidth(), config.kGridHeight());
  Controller controller;

  Game game(config.kGridWidth(), config.kGridHeight());
  game.Run(controller, renderer, config.kMsPerFrame());

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}