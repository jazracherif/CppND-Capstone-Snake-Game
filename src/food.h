#ifndef FOOD_H
#define FOOD_H

#include "SDL.h"


struct FoodColor {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};


class FoodBase {
public:
  SDL_Point point;

public:
  virtual FoodColor getColor() const = 0;
  virtual bool isFriend() const = 0;
  virtual ~FoodBase() = default;
};


class FriendFood : public FoodBase {
public:  
  FoodColor getColor() const {    
    return FoodColor{0x50, 0xC8, 0x78, 0xFF};
  }
  bool isFriend() const {
    return true;
  }

};

class EnemyFood : public FoodBase {
public:
  FoodColor getColor() const {   
    return FoodColor{0xd2, 0x2b, 0x2b, 0xFF};
  }
  bool isFriend() const{
    return false;
  }

};

#endif