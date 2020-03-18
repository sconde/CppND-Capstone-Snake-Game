#ifndef A_STAR_HPP
#define A_STAR_HPP 

#include <vector>
#include <limits>
#include "SDL.h"


enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};


class Node
{
private:
  

public:
  Node *parent = nullptr;
  int x = 0;
  int y = 0;
  int g_cost = 0;
  int h_value = std::numeric_limits<int>::max(); 
  State state = State::kEmpty;
};

class AStar
{
public:
  // default ctor
  //AStar() = default;

  // ctor
  AStar(const std::size_t grid_w, std::size_t grid_h);

  // dtor
  virtual ~AStar ();

  std::vector<SDL_Point> search(const SDL_Point &start, const SDL_Point &end);

  inline int Heuristic(int x1, int y1, int x2, int y2){
    return std::abs(x2 - x1) + std::abs(y2 - y1);
  }

  bool IsInRange(const int x, const int y);

private:
  
  std::vector<std::vector<Node> > nodes_;
  std::vector<Node> open_lists_ {};

  // directional deltas
  constexpr static int delta_[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

};

#endif /* ifndef A_STAR_HPP */
