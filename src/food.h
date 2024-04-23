#ifndef FOOD_H
#define FOOD_H

#include "SDL.h"
#include <chrono>
#include <thread>
#include <random>

class Game;

struct FoodColor {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};


class FoodBase {
public:
  FoodBase(): createdTS(std::chrono::system_clock::now()) {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(5, 10);
    
    validForSeconds = uniform_dist(e1);
  };

public:
  SDL_Point point;
  std::chrono::time_point<std::chrono::system_clock> createdTS;
  long validForSeconds;

public:
  virtual FoodColor getColor() const = 0;
  virtual bool isFriend() const = 0;
  virtual ~FoodBase() = default;
};


class FriendFood : public FoodBase {
public:  
  FoodColor getColor() const;
  bool isFriend() const;

};

class EnemyFood : public FoodBase {
public:
  FoodColor getColor() const;
  bool isFriend() const;
};

#endif