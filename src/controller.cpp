#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
    Snake::Direction opposite) {
  std::lock_guard<std::mutex> lck(mutex_);
  if (snake.GetDirection() != opposite || snake.GetSize() == 1) {
    snake.SetDirection(input);
  }


  return;
}

void Controller::HandleInput(bool &running, Snake &snake, SNAKE_MOVE &move) {
  SDL_Event e;


  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } 

    else /*if (e.type == SDL_KEYDOWN) i*/{

      std::cout << "SIDAFA: controller::HandleInput"
                << "(" << move.x << "," << move.y << ")" <<  std::endl;

      if ( move.x < 0 ) e.key.keysym.sym = SDLK_UP;
      else if ( move.x > 0 ) e.key.keysym.sym = SDLK_DOWN;
      else if ( move.y < 0 ) e.key.keysym.sym = SDLK_LEFT;
      else  e.key.keysym.sym = SDLK_RIGHT;

      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
              Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
              Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
              Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
              Snake::Direction::kLeft);
          break;
      }

    }


  }

}
