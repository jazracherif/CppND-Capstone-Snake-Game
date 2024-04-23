#include "food.h"
#include <iostream>
#include "util.h"
#include "game.h"

 FoodColor FriendFood::getColor() const {    
    return FoodColor{0x50, 0xC8, 0x78, 0xFF};
}

bool FriendFood::isFriend() const {
    return true;
}

FoodColor EnemyFood::getColor() const {   
    return FoodColor{0x00, 0x00, 0x00, 0xFF};
}


bool EnemyFood::isFriend() const {
    return false;
}
