#include "game.h"
#include <iostream>
#include "SDL.h"
#include <algorithm>
#include "gameLogger.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {

  PlaceFood(false);
  PlaceFood(true);
}

Game::~Game(){
  Game::running = false;
  std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  Game::running = true;

  threads.emplace_back(std::thread(&GameLogger::main, std::ref(GameLogger::getLogger()), std::ref(Game::running)));

  while (Game::running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(Game::running, snake);
    Update();
    renderer.Render(snake, foods);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

bool Game::foodAt(int x, int y){
    for(auto &food: foods){
      if (food->point.x == x && food->point.y == y)
        return true;
    }
    return false;
}


void Game::PlaceFood(bool enemy) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake or food item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !Game::foodAt(x, y)) {
      std::unique_ptr<FoodBase> food;
      if (enemy == false) food = std::make_unique<FriendFood>();
      else food = std::make_unique<EnemyFood>();

      food->point.x = x;
      food->point.y = y;
      foods.push_back(std::move(food));
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  // std::cout << "food size " << foods.size() << std::endl;

  bool placeFood = false;
  for (std::vector<std::unique_ptr<FoodBase>>::iterator it = foods.begin(); it != foods.end();) { 

    if ((*it)->point.x == new_x && (*it)->point.y == new_y) {
      std::cout << "::game - Got Food!" << std::endl;
      if (!(*it)->isFriend()){
        std::cout << "::game - enemy food" << std::endl;
      }

      score++;
      // Remove this food item from the list. it has been eaten
      it = foods.erase(it);

      // Grow snake and increase speed.
      snake.GrowBody();
      snake.speed += 0.02;

      GameLogger::getLogger().logEatEvent(score, snake.size, snake.speed);

      // There can only be one food item in one place
      PlaceFood(false);
      break;
    }
    else {
      ++it;
    }
  };

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }