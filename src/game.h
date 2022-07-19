#ifndef GAME_H
#define GAME_H

#include <memory>
#include <thread>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::string user_name_in, std::size_t grid_width, std::size_t grid_height, 
std::shared_ptr<Renderer> sprenderer_in,
std::shared_ptr<Controller> spcontroller_in
);
  void Run(std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  std::string GetUserName() const;

 private:
  std::shared_ptr<Snake> snake;
  //SDL_Point food;
  // neu
  std::shared_ptr<Food> food;
  // alt
  /*Food; food;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;*/

  int score{0};
  std::string user_name{"Snake"};

  //TK neu: nach Food umgezogen
  //void PlaceFood();

  //void Update();

  // temp
  std::shared_ptr<Renderer> sprenderer;
  std::shared_ptr<Controller> spcontroller;
};

#endif