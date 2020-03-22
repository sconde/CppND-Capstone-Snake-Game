#include "game.h"
#include <iostream>
#include "SDL.h"


Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake_(grid_width, grid_height),
      engine_(dev_()),
      random_w_(0, static_cast<int>(grid_width - 1)),
      random_h_(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Renderer &renderer,
               std::size_t target_frame_duration,
               const std::size_t grid_width, const std::size_t grid_height) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  SNAKE_MOVE snake_movement;

  while (running) {
  //for (int i = 0; i < 1; ++i) {
    
    frame_start = SDL_GetTicks();


    // Input, Update, Render - the main game loop.
    controller_.HandleInput(running, snake_, snake_movement);
    Update();
    a_star_.reset(new AStar(grid_width, grid_height));
    // AStar a_star(grid_width, grid_height);
    SDL_Point start, end;
    start.x = snake_.GetHeadY();
    start.y = snake_.GetHeadX();
    end.x = food_.y;
    end.y = food_.x;

    snake_movement.x = end.x;
    snake_movement.y = end.y;

    const auto path = a_star_->Search(start, end);

    snake_movement.x = path[1].x;
    snake_movement.y = path[1].y;

    std::cout << "SIDAFA: start: ("<< start.x << "," << start.y << ")" << std::endl;
    std::cout << "SIDAFA: end: ("<< end.x << "," << end.y << ")" << std::endl;
    std::cout << "SIDAFA: snake_movement: ("<< snake_movement.x << "," << snake_movement.y << ")" << std::endl;
    std::cout << "SIDAFA: snake_head: ("<< snake_.GetHeadY() << "," << snake_.GetHeadX() << ")" << std::endl;
    std::cout << "SIDAFA: speed = " << snake_.GetSpeed()<< std::endl;


    SNAKE_MOVE move;
    const int dy = end.y - snake_movement.y;
    const int dx = end.x - snake_movement.x;
    

    //asm("int $3");
    //std::cout << "SIDAFA: got here!!" << std::endl;

    if (start.y != snake_movement.y) snake_.SetHeadY( snake_movement.y );
    if (start.x != snake_movement.x) snake_.SetHeadX( snake_movement.x );
    snake_.UpdateHead();
    std::cout << "SIDAFA: snake_head(update): ("<< snake_.GetHeadY() << "," << snake_.GetHeadX() << ")" << std::endl;

    renderer.Render(snake_, food_, path, snake_movement);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score_, frame_count);
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

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w_(engine_);
    y = random_h_(engine_);
    // Check that the location is not occupied by a snake_ item before placing
    // food_.
    if (!snake_.SnakeCell(x, y)) {
      food_.x = x;
      food_.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake_.GetAlive()) return;

  snake_.Update();

  //asm("int $3");
  //std::cout << "SIDAFA: got here!!" << std::endl;


  int new_x = static_cast<int>(snake_.GetHeadX());
  int new_y = static_cast<int>(snake_.GetHeadY());

  // Check if there's food_ over here
  if (food_.x == new_x && food_.y == new_y) {
    score_++;
    PlaceFood();
    // Grow snake_ and increase speed.
    snake_.GrowBody();
    snake_.SetSpeed(snake_.GetSpeed());
  }
}

int Game::GetScore() const { return score_; }
int Game::GetSize() const { return snake_.GetSize(); }
