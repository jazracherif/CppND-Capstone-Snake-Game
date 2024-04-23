#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "food.h"

#include <thread>
#include <vector>
#include <mutex>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();

  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  bool isRunning() { return running;}
  
 private:
  Snake snake;
  std::vector<std::unique_ptr<FoodBase>> foods;
  static std::recursive_mutex _foods_mtx; // protect foods structure

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood(bool enemy);
  bool foodAt(int x, int y); // Whether there is food at this location

  void Update();
  void FoodCheck(); // Check whether food has expired
  void FoodCreate(); // Ceate food at random times


  std::vector<std::thread> threads;
  bool running{false}; // whether the game is running

};

#endif