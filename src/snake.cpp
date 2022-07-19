#include "snake.h"
#include <cmath>
#include <iostream>
#include <stack>

struct FoodPortion{
        int x;
        int y;
};
std::stack<FoodPortion> foodportion;
std::mutex m;
std::condition_variable cond;

std::string Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  //TK get coordinates of food
  std::unique_lock<std::mutex> lck(m);
      FoodPortion snake_fp = foodportion.top();
      lck.unlock();
  //std::cout<<"snake_fp x: "<<snake_fp.x<<std::endl;
  //std::cout<<"snake_fp y: "<<snake_fp.y<<std::endl;
  // Update all of the body vector items if the snake head has moved to a new
  // cell.

  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }

  //TK request to Food that FoodLoccation has to be updated
    int new_x = static_cast<int>(head_x);
    int new_y = static_cast<int>(head_y);
  if (snake_fp.x == new_x && snake_fp.y== new_y)
  {
    std::cout<<"Request to update food!"<<std::endl;
    cond.notify_all();
    
  }
  //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return "pico!";
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

bool Snake::UpdateAll(std::shared_ptr<Food> food) {
  bool scored = false;
  if (!alive) return scored;

  //Update();
  // das funtioniert
  //  std::thread t = std::thread(&Snake::Update, this);
  //  t.join();
  std::future<std::string> f1 = std::async(&Snake::Update, shared_from_this());
  std::string ans = f1.get();
  std::cout<<"In Snake: "<<ans<<std::endl;
  int new_x = static_cast<int>(head_x);
    int new_y = static_cast<int>(head_y);
  // Check if there's food over here
  if (food->get_coord_x() == new_x && food->get_coord_y() == new_y) {
    //score++;
    food->PlaceFood(shared_from_this());
    // Grow snake and increase speed.
    GrowBody();
    speed += 0.02;
    scored = true;
  }
  return scored;
}


void Food::PlaceFood(std::shared_ptr<Snake> snake) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      food_coord.x = x;
      food_coord.y = y;
      return;
    }
  }
}
void Food::PlaceNewFood() {
  //int x, y;
  while (true) {
      std::unique_lock<std::mutex> lck(m);
      cond.wait(lck);
      newfood_coord.x = random_w(engine);
      newfood_coord.y = random_h(engine);
      FoodPortion fp;
      fp.x= food_coord.x;
      fp.y= food_coord.y;
      foodportion.push(fp);
      std::cout<<"Setze neue Food koord:"<<std::endl;
      std::cout<<"Neux :"<<fp.x<<std::endl;
      std::cout<<"Neux :"<<fp.y<<std::endl;
      lck.unlock();
      
      return;
 }
}
