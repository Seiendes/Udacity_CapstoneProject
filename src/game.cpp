#include "game.h"
#include <iostream>
#include <thread>
#include "SDL.h"


Game::Game(std::string user_name_in,std::size_t grid_width, std::size_t grid_height, 
std::shared_ptr<Renderer> sprenderer_in,
std::shared_ptr<Controller> spcontroller_in)
    : user_name(user_name_in)
       {
      sprenderer = sprenderer_in;
      //spcontroller = std::move(spcontroller_in);
      spcontroller = spcontroller_in;
      snake = std::make_shared<Snake>(grid_width, grid_height);
      food = std::make_shared<Food>(grid_width, grid_height);     
  //PlaceFood();
}

void Game::Run(std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  food->PlaceFood(snake);
  while (running) {
    frame_start = SDL_GetTicks();

    std::thread tplacefood(&Food::PlaceNewFood, food);
    tplacefood.join();
    // Input, Update, Render - the main game loop.
    /* org */
    // alt ohne thread
    spcontroller->HandleInput(running, snake);
    //neu mit thread
    //std::thread t = std::thread(&Controller::HandleInput, spcontroller, running, std::ref(snake));
    //t.join();
    //TK alt nach snake verschoben
    if(snake->UpdateAll(food)){
      score++;
    };

  //test
  //std::cout<<"newfood x: "<<food->get_newcoord_x()<<std::endl;
  //std::cout<<"newfood y: "<<food->get_newcoord_y()<<std::endl;

    sprenderer->Render(snake, food);
    // neu
    // running  = snake.Run();

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      sprenderer->UpdateWindowTitle(score, frame_count);
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

//TK alt: nach Food umgezogen
/*void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }*/

// TK alt nach snake verschoben
/*
void Game::Update() {
  if (!snake->alive) return;

  snake->Update();

  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);

  // Check if there's food over here
  if (food->food_coord.x == new_x && food->food_coord.y == new_y) {
    score++;
    food->PlaceFood(snake);
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->speed += 0.02;
  }
}*/

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake->get_size(); }
std::string Game::GetUserName() const { return user_name; }