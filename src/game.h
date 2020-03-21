#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "a_star.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Renderer &renderer,
           std::size_t target_frame_duration,
           const std::size_t grid_width, const std::size_t grid_height);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake_;
  Controller controller_;
  SDL_Point food_;
  std::unique_ptr<AStar> a_star_;

  std::random_device dev_;
  std::mt19937 engine_;
  std::uniform_int_distribution<int> random_w_;
  std::uniform_int_distribution<int> random_h_;

  int score_{0};

  void PlaceFood();
  void Update();
};

#endif
