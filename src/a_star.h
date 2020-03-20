#ifndef A_STAR_H
#define A_STAR_H

#include <vector>
#include "SDL.h"
#include <limits>

using std::vector;

enum class State {
    kEmpty = 0,
    kClosed = 1,
    kOpen = 2,
    kPath = 3
};

class Node {
public:
    Node *parent = nullptr;
    int x = 0;
    int y = 0;
    int g_cost = 0;
    int h_cost = std::numeric_limits<int>::max();
    State state = State::kEmpty;
};

class AStar {
public:
    AStar(const std::size_t grid_width, const std::size_t grid_height);
    ~AStar();
    vector<SDL_Point> Search(const SDL_Point &start_point, const SDL_Point &end_point);
private:
    vector<vector<Node>> nodes_;
    vector<Node> open_list_{};
    int delta_[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
    int Heuristic(const int start_point_x, const int start_point_y, const int end_point_x, const int end_point_y);
    bool IsInRange(const int x, const int y);
};

#endif
