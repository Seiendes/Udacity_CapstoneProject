#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <future>
#include <thread>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "SDL.h"


// Declaration of functions in main
std::string get_UserName();
void write_HighScore(std::shared_ptr<Game> game);


int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  // get user name for highscore list
  std::string user_name = get_UserName();


  // TK **rubric: usage of smart pointer **


  //std::unique_ptr<Controller> controller = std::make_unique<Controller>();
  // TK **rubric: define game as non-smart pointer for usage of ~Game()
  //Game *game = new Game(user_name, kGridWidth, kGridHeight,
  //                      controller, renderer);

  // TK: start task with simulation of game instance
  // schon vorbereitet fuer threads
  //std::future<void> ftr = std::async(&Game::Run, game, std::move(controller), std::move(renderer), kMsPerFrame);
  //game->Run(kMsPerFrame);

  // neu
  std::shared_ptr<Renderer> sprenderer = std::make_shared<Renderer>(kScreenWidth, 
                                                    kScreenHeight, kGridWidth, kGridHeight);

//alt
  //Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  //Controller controller;
  std::unique_ptr<Controller> controller = std::make_unique<Controller>();
  std::shared_ptr<Game> game = std::make_unique<Game>(user_name, kGridWidth, kGridHeight,
  sprenderer, std::move(controller));
  // TK old game without thread
  //game->Run(kMsPerFrame);
  // new: game with thread
  std::thread t = std::thread(&Game::Run, game, kMsPerFrame);
  t.join();

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game->GetScore() << "\n";
  std::cout << "Size: " << game->GetSize() << "\n";


  write_HighScore(game);

  // release memory and call destructor;
  //delete game;
  return 0;
}


// TK **rubric: organization into functions**
// get user name from terminal
std::string get_UserName(){
  std::cout<<"Before gaming, please enter user name"<<std::endl;
  std::string user_name;
  std::cin>>user_name;
  return user_name;
}

// TK **rubric: read from file, write to file, different control structures**
// update highsore list
void write_HighScore(std::shared_ptr<Game> game)
{
  {
    std::string filename("highscore.dat");
    std::fstream file_out;

    file_out.open(filename, std::ios_base::out);
    if (!file_out.is_open()) {
        std::cout << "failed to open " << filename << '\n';
    } else {
        file_out << 10 << "uwe"<<std::endl;
        file_out << 5 << "thomas"<<std::endl;
        file_out << 3 << "asd"<<std::endl;
    }
    file_out.close();}

    // Input data
    std::string user_name = "thomas";
    int user_score = 11;

    // Open the highscore data file
    std::fstream file_in;
    file_in.open("./highscore.dat");
    if(!file_in){
        std::cerr<<"In-File wasn't readable!";
    }
    // Define a vector which stores the file input
    struct hscore_line{
        int score;
        std::string name;
    };
    std::vector<hscore_line> hscore;

    // Read scores and name from filestream
    std::string line;
    while(getline(file_in, line)){ 
        std::istringstream mystream(line);
        std::string pers_name;
        int pers_hscore;
        mystream >>pers_hscore>>pers_name;
        hscore.push_back({pers_hscore, pers_name});
    }
    file_in.close();
    
    // Add current score 
    // If user exists already: store max. value and remove from sorted list
    hscore_line dummy{game->GetScore(), game->GetUserName()}; 
    for(std::vector<hscore_line>::iterator it = hscore.begin(); it != hscore.end();it++){
        if(it->name.compare(dummy.name)==0){
            dummy.score = std::max(dummy.score, it->score);
            it = hscore.erase(it);
            break;
        }
    }
    // Insert at proper position
    bool IsInsert = false;
    for(std::vector<hscore_line>::iterator it = hscore.begin(); it != hscore.end();it++){
        if(it->score<dummy.score){
            hscore.insert(it, dummy);
            IsInsert = true;
            break;
        }
    }
    if(not IsInsert){
        hscore.push_back(dummy);
    }

    std::string filename("./highscore.dat");
    std::fstream file_out;
    file_out.open("./highscore.dat");
    if(!file_out){
        std::cerr<<"Out-File wasn't readable!";
    }
    else {
        for(auto &it: hscore)
        {
            file_out <<it.score<<it.name<< std::endl;
        }
        std::cout << "Highscores updated, leader is: " << std::endl;
        std::cout << hscore[0].name<<": "<<hscore[0].score<<" scores!" << std::endl;
    }
    file_out.close();
}