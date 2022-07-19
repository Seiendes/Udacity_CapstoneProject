#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <memory>
#include <thread>
#include <future>
#include "SDL.h"
#include <random>
#include <string>
class Food;
class Snake : public std::enable_shared_from_this<Snake>
{
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {
        }

  bool UpdateAll(std::shared_ptr<Food> food);
  std::string Update();
  void GrowBody();
  bool SnakeCell(int x, int y);
  int get_size(){return size;};
  Direction get_direction(){return direction;};
  void set_direction(Direction input){direction=input;};
  std::vector<SDL_Point> get_body(){return body;};
  float get_head_x(){return head_x;};
  float get_head_y(){return head_y;};
  bool get_alive(){return alive;};

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
};

class Food{
  public:
    Food(int grid_width, int grid_height):
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1))
      {
        //PlaceFood();
      }
    void PlaceFood(std::shared_ptr<Snake> snake);
    int get_coord_x(){return food_coord.x;};
    int get_coord_y(){return food_coord.y;};
        int get_newcoord_x(){return newfood_coord.x;};
    int get_newcoord_y(){return newfood_coord.y;};
    void PlaceNewFood();
  private:
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
    SDL_Point food_coord;
    SDL_Point newfood_coord;
};


#endif