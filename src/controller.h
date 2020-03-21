#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <mutex>
#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake, SNAKE_MOVE &snake_movement) ;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) ;

  std::mutex mutex_;
};

#endif
